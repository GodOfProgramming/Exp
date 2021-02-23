#pragma once

namespace Exp
{
  namespace Game
  {
    class Camera
    {
      Camera() = default;

     public:
      Camera(const Camera&) = delete;
      Camera(Camera&&)      = delete;
      auto operator=(const Camera&) -> Camera& = delete;
      auto operator=(Camera&&) -> Camera& = delete;

      static auto instance() noexcept -> Camera&;

      static void add_usertype(sol::state_view state);

      void set_ortho(float left, float right, float bottom, float top, float near, float far);

      void move(glm::vec3 dir);

      auto get_projection() -> glm::mat4;

      auto get_view() -> glm::mat4;

     private:
      glm::mat4 proj;
      glm::vec3 view;
    };
  }  // namespace Game
}  // namespace Exp