#pragma once

#include "program.hpp"

namespace Exp
{
  namespace GL
  {
    class Uniform
    {
      enum class Type
      {
        Float,
        Vec2,
        Vec3,
        Vec4,
        FloatV,
        Vec2V,
        Vec3V,
        Vec4V,
      };

     public:
      Uniform(std::string name);
      static auto create(std::string name) -> std::shared_ptr<Uniform>;

      void set_float(float f);
      void set_vec2(glm::vec2 v);
      void set_vec3(glm::vec3 v);
      void set_vec4(glm::vec4 v);

      void set_v_float(std::vector<float> v);
      void set_v_vec2(std::vector<glm::vec2> v);
      void set_v_vec3(std::vector<glm::vec3> v);
      void set_v_vec4(std::vector<glm::vec4> v);

      auto enable(const Program& shader) const -> bool;

      static void add_usertype(sol::state_view state);

      const std::string name;

     private:
      std::variant<float, glm::vec2, glm::vec3, glm::vec4, std::vector<float>, std::vector<glm::vec2>, std::vector<glm::vec3>, std::vector<glm::vec4>> value;

      auto type() const noexcept -> Type;
    };
  }  // namespace GL
}  // namespace Exp
