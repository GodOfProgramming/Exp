#include "object.hpp"

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
        using Resources::Scripts;

        auto& scripts = Scripts::instance();
        scripts.make_script(m.script_id.value(), this->script, [](sol::state& state) {
          ObjectMeta::add_usertype(state);
          Uniform::add_usertype(state);
          Object::add_usertype(state);
          Info::add_usertype(state);
          return true;
        });

        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua["Instantiate"];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }
      }
    }

    void Object::prerender()
    {
      for (const auto& kvp : this->uniforms) {
        auto& uniform = kvp.second;
        if (!uniform.enable(*this->meta.shader)) {
          LOG(WARNING) << "unable to enable uniform " << uniform.name << " on object " << this->meta.id;
        }
      }
    }

    void Object::update()
    {
      if (this->script.has_value()) {
        auto& lua = this->script.value();
        auto fn   = lua["Update"];
        if (fn.get_type() == sol::type::function) {
          fn.call(this);
        }
      }
    }

    void Object::add_usertype(sol::state& state)
    {
      state.new_usertype<Object>("Object", "meta", &Object::meta, "uniforms", &Object::uniforms);
    }
  }  // namespace Game
}  // namespace Exp