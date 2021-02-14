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
      };

     public:
      Uniform(std::string name);
      void set_float(float f);
      void set_vec2(float x, float y);
      void set_vec3(float x, float y, float z);
      void set_vec4(float x, float y, float z, float w);

      auto enable(const Program& shader) const -> bool;

      static void add_usertype(sol::state& state);

      const std::string name;

     private:
      std::variant<float, glm::vec2, glm::vec3, glm::vec4> value;

      auto type() const noexcept -> Type;
    };
  }  // namespace GL
}  // namespace Exp
