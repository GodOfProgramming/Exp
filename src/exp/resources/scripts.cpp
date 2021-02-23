#include "scripts.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"
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
      const static std::string libdir = std::string(Assets::Dir::GAME_SCRIPTS) + "/lib";
      IO::iterate_dir_with_namespace(Assets::Dir::GAME_SCRIPTS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        LOG(INFO) << "loading script " << nspace;
        bool loaded = false;

        this->load_src_file(path, [&](const std::string_view& src) {
          ScriptMeta meta;
          meta.src = src;

          this->scripts.emplace(nspace, std::move(meta));
          loaded = true;
        });

        if (!loaded) {
          LOG(WARNING) << "unable to load script";
        }
      });
    }

    void Scripts::release()
    {
      this->scripts.clear();
    }

    auto Scripts::find(std::string id) const noexcept -> ScriptMap::const_iterator
    {
      return this->scripts.find(id);
    }

    auto Scripts::begin() const noexcept -> ScriptMap::const_iterator
    {
      return this->scripts.begin();
    }

    auto Scripts::end() const noexcept -> ScriptMap::const_iterator
    {
      return this->scripts.end();
    }

    auto Scripts::make_script(std::string id, sol::state& state, std::function<bool(sol::state_view&)> callback) -> bool
    {
      auto iter = this->find(id);
      if (iter == this->end()) {
        LOG(WARNING) << "could not find script " << id;
        return false;
      }

      sol::state lua;
      lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::package);

      const std::string package_path      = lua["package"]["path"];
      static const std::string script_lib = std::string(";") + Assets::Dir::GAME_SCRIPTS + "/lib/?.lua";
      lua["package"]["path"]              = package_path + script_lib;

      add_usertype<glm::mat4>(lua);
      add_usertype<glm::vec2>(lua);
      add_usertype<glm::vec3>(lua);
      add_usertype<glm::vec4>(lua);
      add_usertype<glm::ivec2>(lua);

      GameObjects::add_usertype(lua);

      if (!callback(lua)) {
        LOG(WARNING) << "make script callback returned false";
        return false;
      }

      auto res = lua.load(iter->second.src);
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

      state = std::move(lua);
      return true;
    }
  }  // namespace R
}  // namespace Exp