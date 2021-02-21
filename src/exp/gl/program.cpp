#include "program.hpp"

#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    Program::Program(const std::string i)
     : id(i)
     , valid(false)
    {
      this->pid = glCreateProgram();
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to create new shader (gl error != GL_NO_ERROR)";
      }
    }

    Program::~Program()
    {
      if (this->pid != 0) {
        glDeleteProgram(this->pid);
        GL_CHECK();
      }
    }

    void Program::add_usertype(sol::state_view state)
    {
      state.new_usertype<Program>("ShaderProgram", "id", &Program::id);
    }

    auto Program::attach(const Shader& shader) -> bool
    {
      if (this->pid == 0) {
        LOG(ERROR) << "unable to link program (program id == 0)";
        return false;
      }

      if (!shader.is_valid()) {
        LOG(ERROR) << "tried linking invalid shader";
        return false;
      }

      glAttachShader(this->pid, shader.shader_id());
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to attach shader";
        return false;
      }

      return true;
    }

    auto Program::link(std::string& errstr) -> bool
    {
      bool result = true;

      glLinkProgram(this->pid);
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to link shader program";
        result = false;
      }

      GLint success;
      glGetProgramiv(this->pid, GL_LINK_STATUS, &success);
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to get program iv";
        result = false;
      }

      if (success == GL_FALSE) {
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetProgramInfoLog(this->pid, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "unable to get compilation errors for shader";
        }
        errstr.assign(info_log.data(), static_cast<std::size_t>(len));
        result = false;
      }

      return this->valid = result;
    }

    auto Program::program_id() const noexcept -> ProgramID
    {
      return this->pid;
    }

    auto Program::is_valid() const noexcept -> bool
    {
      return this->valid;
    }

    auto Program::use() -> bool
    {
      glUseProgram(this->pid);
      return GL_CHECK();
    }
  }  // namespace GL
}  // namespace Exp
