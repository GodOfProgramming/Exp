#include "uniform.hpp"

namespace Exp
{
  namespace GL
  {
    Uniform::Uniform(const std::string_view n)
     : name(n)
    {}

    void Uniform::set_float(float f)
    {
      this->value = f;
    }

    void Uniform::set_vec2(float x, float y)
    {
      this->value = glm::vec2{ x, y };
    }

    void Uniform::set_vec3(float x, float y, float z)
    {
      this->value = glm::vec3{ x, y, z };
    }

    void Uniform::set_vec4(float x, float y, float z, float w)
    {
      this->value = glm::vec4{ x, y, z, w };
    }
  }  // namespace GL
}  // namespace Exp
