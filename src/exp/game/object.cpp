#include "object.hpp"

#include "camera.hpp"
#include "exp/constants.hpp"
#include "exp/resources/producer.hpp"
#include "exp/resources/scripts.hpp"
#include "info.hpp"

namespace Exp
{
  namespace Game
  {
    Object::Object(const ObjectMeta m, glm::vec3 p)
     : meta(m)
     , pos(p)
    {
      this->obj_id = R::ID<std::size_t>::producer_t::instance().produce();

      if (!m.script_id.empty()) {
        using R::Scripts;

        auto& scripts = Scripts::instance();
        sol::environment lua;
        if (scripts.create_env(m.script_id, lua)) {
          this->env = std::move(lua);
        }

        if (this->env.has_value()) {
          auto& lua = this->env.value();
          auto fn   = lua[this->meta.construct_fn];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }
      }
    }

    Object::~Object()
    {
      this->obj_id.release();
    }

    void Object::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Game::OBJECT].get_type() == sol::type::none) {
        Camera::add_usertype(state);
        ObjectMeta::add_usertype(state);
        Uniform::add_usertype(state);
        state.new_usertype<Object>(
         Lua::Usertypes::Game::OBJECT, "meta", &Object::meta, "uniforms", &Object::uniforms, "pos", &Object::pos, "id", &Object::id, "data", &Object::data);
      }
    }

    void Object::prerender()
    {
      for (const auto& kvp : this->uniforms) {
        auto& uniform = kvp.second;
        uniform->enable(*this->meta.shader);
      }

      if (this->meta.animation != nullptr) {
        if (!this->meta.animation->texture->tex->bind()) {
          return;
        }
      }
    }

    void Object::update()
    {
      if (this->env.has_value()) {
        auto& lua = this->env.value();
        auto fn   = lua[this->meta.update_fn];
        if (fn.get_type() == sol::type::function) {
          fn.call(this);
        }
      }
    }

    auto Object::id() const noexcept -> std::size_t
    {
      return this->obj_id.value();
    }

    auto Object::location() const noexcept -> glm::vec3
    {
      return this->pos;
    }
  }  // namespace Game
}  // namespace Exp