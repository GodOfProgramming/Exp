#include "uniform.hpp"

#include "exp/constants.hpp"
#include "exp/gl/error_map.hpp"

namespace Exp
{
  namespace GL
  {
    Uniform::Uniform(std::string n)
     : name(n)
    {}

    auto Uniform::create(std::string name) -> std::shared_ptr<Uniform>
    {
      return std::make_shared<Uniform>(name);
    }

    void Uniform::set_float(float f)
    {
      this->value = f;
    }

    void Uniform::set_vec2(glm::vec2 v)
    {
      this->value = v;
    }

    void Uniform::set_vec3(glm::vec3 v)
    {
      this->value = v;
    }

    void Uniform::set_vec4(glm::vec4 v)
    {
      this->value = v;
    }

    void Uniform::set_v_float(std::vector<float> v)
    {
      this->value = v;
    }

    void Uniform::set_v_vec2(std::vector<glm::vec2> v)
    {
      this->value = v;
    }

    void Uniform::set_v_vec3(std::vector<glm::vec3> v)
    {
      this->value = v;
    }

    void Uniform::set_v_vec4(std::vector<glm::vec4> v)
    {
      this->value = v;
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
          glUniform3fv(loc, 1, glm::value_ptr(std::get<glm::vec3>(this->value)));
        } break;
        case Type::Vec4: {
          glUniform4fv(loc, 1, glm::value_ptr(std::get<glm::vec4>(this->value)));
        } break;
        case Type::FloatV: {
          auto& v = std::get<std::vector<float>>(this->value);
          if (v.size() > 0) {
            glUniform1fv(loc, v.size(), v.data());
          } else {
            return false;
          }
        } break;
        case Type::Vec2V: {
          auto& vecs = std::get<std::vector<glm::vec2>>(this->value);
          if (vecs.size() > 0) {
            glUniform2fv(loc, vecs.size(), glm::value_ptr(vecs[0]));
          } else {
            return false;
          }
        } break;
        case Type::Vec3V: {
          auto& vecs = std::get<std::vector<glm::vec3>>(this->value);
          if (vecs.size() > 0) {
            glUniform3fv(loc, vecs.size(), glm::value_ptr(vecs[0]));
          } else {
            return false;
          }
        } break;
        case Type::Vec4V: {
          auto& vecs = std::get<std::vector<glm::vec4>>(this->value);
          if (vecs.size() > 0) {
            glUniform4fv(loc, vecs.size(), glm::value_ptr(vecs[0]));
          } else {
            return false;
          }
        } break;
        default: {
          LOG(WARNING) << "invalid shader datatype";
          return false;
        }
      }

      return GL_CHECK();
    }

    void Uniform::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::GL::UNIFORM].get_type() == sol::type::none) {
        state.new_usertype<Uniform>(
         Lua::Usertypes::GL::UNIFORM,
         "create",
         &Uniform::create,
         "set_float",
         &Uniform::set_float,
         "set_vec2",
         &Uniform::set_vec2,
         "set_vec3",
         &Uniform::set_vec3,
         "set_vec4",
         &Uniform::set_vec4,
         "set_v_float",
         &Uniform::set_v_float,
         "set_v_vec2",
         &Uniform::set_v_vec2,
         "set_v_vec3",
         &Uniform::set_v_vec3,
         "set_v_vec4",
         &Uniform::set_v_vec4);
      }
    }

    auto Uniform::type() const noexcept -> Type
    {
      return static_cast<Type>(this->value.index());
    }
  }  // namespace GL
}  // namespace Exp
