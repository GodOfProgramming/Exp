#include "uniform.hpp"

#include "exp/gl/error_map.hpp"

namespace Exp
{
  namespace GL
  {
    Uniform::Uniform(std::string n)
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

    auto Uniform::enable(const GL::Program& program) const -> bool
    {
      auto loc = glGetUniformLocation(program.program_id(), this->name.c_str());
      if (!GL_CHECK()) {
        return false;
      }

      if (loc < 0) {
        return false;
      }

      switch (this->type()) {
        case Type::Float: {
          glUniform1fv(loc, 1, &std::get<float>(this->value));
        } break;
        case Type::Vec2: {
          glUniform2fv(loc, 1, glm::value_ptr(std::get<glm::vec2>(this->value)));
        } break;
        case Type::Vec3: {
          glUniform2fv(loc, 1, glm::value_ptr(std::get<glm::vec3>(this->value)));
        } break;
        case Type::Vec4: {
          glUniform4fv(loc, 1, glm::value_ptr(std::get<glm::vec4>(this->value)));
        } break;
        default: {
          return false;
        }
      }
      return GL_CHECK();
    }

    void Uniform::add_usertype(sol::state& state)
    {
      state.new_usertype<Uniform>(
       "Uniform",
       sol::constructors<Uniform(std::string)>(),
       "set_float",
       &Uniform::set_float,
       "set_vec2",
       &Uniform::set_vec2,
       "set_vec3",
       &Uniform::set_vec3,
       "set_vec4",
       &Uniform::set_vec4);
    }

    auto Uniform::type() const noexcept -> Type
    {
      return static_cast<Type>(this->value.index());
    }
  }  // namespace GL
}  // namespace Exp
