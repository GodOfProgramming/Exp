#include "object.hpp"

#include "exp/constants.hpp"
#include "exp/resources/scripts.hpp"
#include "info.hpp"

namespace Exp
{
  namespace Game
  {
    Object::Object(const ObjectMeta m)
     : meta(m)
    {
      if (m.script_id.has_value()) {
        using R::Scripts;

        auto& scripts = Scripts::instance();
        sol::state lua;
        if (scripts.make_script(m.script_id.value(), lua, [this](sol::state_view& state) {
              ObjectMeta::add_usertype(state);
              Uniform::add_usertype(state);
              Object::add_usertype(state);
              Info::add_usertype(state);
              state.set("self", this);
              return true;
            })) {
          this->script = std::move(lua);
        }

        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua["construct"];
          if (fn.get_type() == sol::type::function) {
            fn.call();
          }
        }
      }
    }

    void Object::prerender()
    {
      for (const auto& kvp : this->uniforms) {
        auto& uniform = kvp.second;
        uniform.enable(*this->meta.shader);
      }
    }

    void Object::update()
    {
      if (this->script.has_value()) {
        auto& lua = this->script.value();
        auto fn   = lua["update"];
        if (fn.get_type() == sol::type::function) {
          fn.call();
        }
      }
    }

    void Object::add_usertype(sol::state_view state)
    {
      state.new_usertype<Object>(Lua::Usertypes::OBJECT, "meta", &Object::meta, "uniforms", &Object::uniforms);
    }
  }  // namespace Game
}  // namespace Exp