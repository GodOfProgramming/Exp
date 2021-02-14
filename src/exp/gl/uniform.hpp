#pragma once

#include "program.hpp"

namespace Exp
{
  namespace GL
  {
    class Uniform
    {
     public:
      Uniform(const std::string_view name);
      void set_float(float f);
      void set_vec2(float x, float y);
      void set_vec3(float x, float y, float z);
      void set_vec4(float x, float y, float z, float w);

      auto enable(const Program& shader) -> bool;

     private:
      const std::string_view name;
      std::variant<float, glm::vec2, glm::vec3, glm::vec4> value;
    };
  }  // namespace GL
}  // namespace Exp
