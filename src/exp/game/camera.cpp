#include "camera.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Game
  {
    auto Camera::instance() noexcept -> Camera&
    {
      static Camera camera;
      return camera;
    }

    void Camera::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Game::CAMERA].get_type() == sol::type::none) {
        state.new_usertype<Camera>(
         Lua::Usertypes::Game::CAMERA,
         "instance",
         &Camera::instance,
         "set_ortho",
         &Camera::set_ortho,
         "move_to",
         &Camera::move_to,
         "get_projection",
         &Camera::get_projection,
         "get_view",
         &Camera::get_view);
      }
    }

    void Camera::set_ortho(float left, float right, float bottom, float top, float near, float far)
    {
      this->proj = glm::ortho(left, right, bottom, top, near, far);
    }

    void Camera::move_to(glm::vec3 loc)
    {
      this->view = loc;
    }

    auto Camera::get_projection() -> glm::mat4
    {
      return this->proj;
    }

    auto Camera::get_view() -> glm::mat4
    {
      return glm::translate(glm::identity<glm::mat4>(), this->view);
    }
  }  // namespace Game
}  // namespace Exp