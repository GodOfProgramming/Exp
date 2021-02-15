#include "scripts.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"

namespace Exp
{
  namespace R
  {
    auto Scripts::instance() noexcept -> Scripts&
    {
      static Scripts scripts;
      return scripts;
    }

    void Scripts::load_all()
    {
      IO::iterate_dir_with_namespace(DIR_GAME_SCRIPTS, std::string{ "exp" }, [&](const std::filesystem::path path, const std::string& nspace) {
        auto file_res = IO::File::load(path);
        if (!file_res) {
          LOG(WARNING) << "unable to load game object configuration file: " << file_res.err_val();
          return;
        }

        auto file = file_res.ok_val();

        LOG(INFO) << "loading script " << nspace;

        ScriptMeta meta;
        meta.src = file.data;
        this->scripts.emplace(nspace, std::move(meta));
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

    void Scripts::make_script(std::string id, std::optional<sol::state>& state, std::function<bool(sol::state&)> callback)
    {
      auto iter = this->find(id);
      if (iter == this->end()) {
        return;
      }

      sol::state lua;

      lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

      lua.new_usertype<glm::vec2>("Vec2", sol::constructors<glm::vec2(), glm::vec2(float, float)>(), "x", &glm::vec2::x, "y", &glm::vec2::y);

      lua.new_usertype<glm::vec3>(
       "Vec3", sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(), "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);

      lua.new_usertype<glm::vec4>(
       "Vec4",
       sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
       "x",
       &glm::vec4::x,
       "y",
       &glm::vec4::y,
       "z",
       &glm::vec4::z,
       "w",
       &glm::vec4::w);

      lua.new_usertype<glm::ivec2>("ivec2", sol::constructors<glm::ivec2(), glm::ivec2(int, int)>(), "x", &glm::ivec2::x, "y", &glm::ivec2::y);

      if (!callback(lua)) {
        return;
      }

      auto res = lua.load(iter->second.src);
      if (!res.valid()) {
        return;
      }

      auto exec_res = res();
      if (!exec_res.valid()) {
        return;
      }

      state = std::move(lua);
    }
  }  // namespace R
}  // namespace Exp