#include "scripts.hpp"

#include "exp/constants.hpp"
#include "exp/game/camera.hpp"
#include "exp/game/info.hpp"
#include "exp/game/object.hpp"
#include "exp/gl/uniform.hpp"
#include "exp/input/enums.hpp"
#include "exp/input/gamepad.hpp"
#include "exp/input/keyboard.hpp"
#include "exp/input/mouse.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"
#include "exp/render/draw_description.hpp"
#include "exp/resources/animation_meta.hpp"
#include "exp/resources/animations.hpp"
#include "exp/resources/game_objects.hpp"
#include "exp/resources/model_meta.hpp"
#include "exp/resources/models.hpp"
#include "exp/resources/object_meta.hpp"
#include "exp/resources/shader_meta.hpp"
#include "exp/resources/shaders.hpp"
#include "exp/ui/components/frame.hpp"
#include "exp/ui/components/text_box.hpp"
#include "exp/ui/components/window_ui.hpp"
#include "game_objects.hpp"

#define NEW_UTYPE(type, name, ...)                    \
  template <>                                         \
  void add_usertype<type>(sol::state_view state)      \
  {                                                   \
    using type_t                = type;               \
    constexpr const char* tname = name;               \
    state.new_usertype<type_t>(tname, ##__VA_ARGS__); \
  }

namespace Exp
{
  namespace
  {
    template <typename T>
    void add_usertype(sol::state_view);

    NEW_UTYPE(
     glm::mat4,
     Lua::Usertypes::glm::MAT4,
     "identity",
     []() -> type_t { return glm::identity<type_t>(); },
     "translate",
     [](glm::mat4 self, glm::vec3 t) -> type_t { return glm::translate(self, t); },
     "rotate",
     [](type_t self, float rads, glm::vec3 quat) -> type_t { return glm::rotate(self, rads, quat); },
     "scale",
     [](type_t self, glm::vec3 scale) -> type_t { return glm::scale(self, scale); });

    NEW_UTYPE(glm::vec2, Lua::Usertypes::glm::VEC2, sol::constructors<type_t(), type_t(float, float)>(), "x", &type_t::x, "y", &type_t::y)

    NEW_UTYPE(
     glm::vec3,
     Lua::Usertypes::glm::VEC3,
     sol::constructors<type_t(), type_t(float, float, float)>(),
     "x",
     &type_t::x,
     "y",
     &type_t::y,
     "z",
     &type_t::z,
     "add",
     [](type_t self, type_t other) -> type_t { return self + other; },
     "mul",
     [](type_t self, type_t other) -> type_t { return self * other; })

    NEW_UTYPE(
     glm::vec4,
     Lua::Usertypes::glm::VEC4,
     sol::constructors<type_t(), type_t(float, float, float, float)>(),
     "x",
     &type_t::x,
     "y",
     &type_t::y,
     "z",
     &type_t::z,
     "w",
     &type_t::w)

    NEW_UTYPE(glm::ivec2, Lua::Usertypes::glm::IVEC2, sol::constructors<type_t(), type_t(int, int)>(), "x", &type_t::x, "y", &type_t::y)
  }  // namespace

  namespace R
  {
    auto Scripts::instance() noexcept -> Scripts&
    {
      static Scripts scripts;
      return scripts;
    }

    void Scripts::load_all()
    {
      LOG(INFO) << "loading scripts";
      IO::iterate_dir_with_namespace(
       std::string(Assets::Dir::GAME_SCRIPTS) + "/src", std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
         LOG(INFO) << "loading script " << nspace;
         bool loaded = false;

         this->load_src_file(path, [&](const std::string_view& src) {
           std::string source(src);
           this->sources.emplace(nspace, std::move(source));
           loaded = true;
         });

         if (!loaded) {
           LOG(WARNING) << "unable to load script source";
           return;
         }

         if (!this->initialize_state(nspace)) {
           LOG(WARNING) << "could not initialize state";
           return;
         }
       });
    }

    void Scripts::release()
    {
      this->states.clear();
      this->sources.clear();
    }

    auto Scripts::initialize_state(std::string id) -> bool
    {
      auto source_iter = this->sources.find(id);
      if (source_iter == this->sources.end()) {
        LOG(WARNING) << "could not find script source for " << id;
        return false;
      }

      sol::state state;
      state.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::package);

      const static std::string libdir = std::string(";") + Assets::Dir::GAME_SCRIPTS + "/lib/?.lua";
      const std::string package_path  = state["package"]["path"];
      state["package"]["path"]        = package_path + libdir;

      add_usertype<glm::mat4>(state);
      add_usertype<glm::vec2>(state);
      add_usertype<glm::vec3>(state);
      add_usertype<glm::vec4>(state);
      add_usertype<glm::ivec2>(state);
      GL::Uniform::add_usertype(state);
      Game::Camera::add_usertype(state);
      Game::Info::add_usertype(state);
      Game::Object::add_usertype(state);
      Input::Gamepad::add_usertype(state);
      Input::Keyboard::add_usertype(state);
      Input::Mouse::add_usertype(state);
      Input::add_usertype<Input::Action>(state);
      Input::add_usertype<Input::GamepadAxis>(state);
      Input::add_usertype<Input::GamepadButton>(state);
      Input::add_usertype<Input::Key>(state);
      Input::add_usertype<Input::MouseButton>(state);
      R::AnimationMeta::add_usertype(state);
      R::GameObjects::add_usertype(state);
      R::ObjectMeta::add_usertype(state);
      Render::DrawDescription::add_usertype(state);
      Ui::Components::Frame::add_usertype(state);
      Ui::Components::TextBox::add_usertype(state);
      Ui::Components::WindowUi::add_usertype(state);

      auto res = state.load(source_iter->second);
      if (!res.valid()) {
        sol::error err = res;
        LOG(WARNING) << "bad script load result:\n" << err.what();
        return false;
      }

      auto exec_res = res();
      if (!exec_res.valid()) {
        sol::error err = exec_res;
        res.status();
        LOG(WARNING) << "bad script exec result:\n" << err.what();
        return false;
      }

      this->states[id] = std::move(state);
      return true;
    }

    auto Scripts::create_env(std::string id, sol::environment& env) -> bool
    {
      auto iter = this->states.find(id);
      if (iter == this->states.end()) {
        LOG(WARNING) << "unable to find script " << id;
        return false;
      }

      auto& state = iter->second;

      env = std::move(sol::environment(state, sol::create, state.globals()));
      return true;
    }
  }  // namespace R
}  // namespace Exp