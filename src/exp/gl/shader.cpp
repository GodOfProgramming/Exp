#include "shader.hpp"

#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    Shader::~Shader() noexcept
    {
      if (this->id != 0) {
        glDeleteShader(this->id);
        GL_CHECK();
      }
    }

    auto Shader::compile(Type t, const std::string& src, std::string& errstr) noexcept -> bool
    {
      this->id = glCreateShader(static_cast<GLenum>(t));
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to create new shader (gl error != GL_NO_ERROR)";
        return false;
      }

      if (this->id == 0) {
        LOG(ERROR) << "unable to create new shader (shader = 0)";
        return false;
      }

      auto data = src.data();
      GLint len = src.length();

      glShaderSource(this->id, 1, &data, &len);
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to set shader source";
        return false;
      }

      bool result = true;

      glCompileShader(this->id);
      if (!GL_CHECK()) {
        LOG(WARNING) << "shader failed to compile";
        result = false;
      }

      GLint success = 0;
      glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not get shader compile status";
        result = false;
      }

      if (success == GL_FALSE) {
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetShaderInfoLog(this->id, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "unable to get compilation errors for shader";
        }
        errstr.assign(info_log.data(), static_cast<std::size_t>(len));
        result = false;
      }

      return this->valid = result;
    }

    auto Shader::shader_id() const noexcept -> ShaderID
    {
      return this->id;
    }

    auto Shader::is_valid() const noexcept -> bool
    {
      return this->valid;
    }
  }  // namespace GL
}  // namespace Exp
