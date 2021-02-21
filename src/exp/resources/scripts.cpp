#include "scripts.hpp"

#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/io/file.hpp"
#include "game_objects.hpp"

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

      lua.new_usertype<glm::vec2>(Lua::Usertypes::glm::VEC2, sol::constructors<glm::vec2(), glm::vec2(float, float)>(), "x", &glm::vec2::x, "y", &glm::vec2::y);

      lua.new_usertype<glm::vec3>(
       Lua::Usertypes::glm::VEC3, sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(), "x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);

      lua.new_usertype<glm::vec4>(
       Lua::Usertypes::glm::VEC4,
       sol::constructors<glm::vec4(), glm::vec4(float, float, float, float)>(),
       "x",
       &glm::vec4::x,
       "y",
       &glm::vec4::y,
       "z",
       &glm::vec4::z,
       "w",
       &glm::vec4::w);

      lua.new_usertype<glm::ivec2>(
       Lua::Usertypes::glm::IVEC2, sol::constructors<glm::ivec2(), glm::ivec2(int, int)>(), "x", &glm::ivec2::x, "y", &glm::ivec2::y);

      Render::DrawDescription::add_usertype(lua);
      ObjectMeta::add_usertype(lua);
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