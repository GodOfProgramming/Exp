#include "gl.hpp"

namespace ExpGame
{
  namespace GL
  {
    auto ErrorMap::instance() -> ErrorMap&
    {
      static ErrorMap err_map;
      return err_map;
    }

    ErrorMap::ErrorMap() {}

    auto ErrorMap::check(const char* file, int line) -> bool
    {
      auto err = glGetError();
      if (err == GL_NO_ERROR) {
        return true;
      } else {
        auto entry = this->errors.find(err);
        if (entry == this->errors.end()) {
          this->errors[err].occurrences[std::string(file)].push_back(line);
        } else {
          auto& entry = this->errors[err];
          entry.desc  = std::to_string(err);
          entry.occurrences[std::string(file)].push_back(line);
        }
        return false;
      }
    }

    auto ErrorMap::begin() -> Iter
    {
      return this->errors.begin();
    }

    auto ErrorMap::end() -> Iter
    {
      return this->errors.end();
    }

    auto VBO::gen() -> bool
    {
      glGenBuffers(1, &this->id);
      return GL_CHECK();
    }

    auto VBO::bind() -> bool
    {
      glBindBuffer(GL_ARRAY_BUFFER, this->id);
      return GL_CHECK();
    }

    Shader::Shader() noexcept
     : id(0)
     , valid(false)
    {}

    Shader::~Shader() noexcept
    {
      if (this->id != 0) {
        glDeleteShader(this->id);
      }
    }

    auto Shader::compile(Type t, const std::string_view src) noexcept -> bool
    {
      this->source = src;

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

      glCompileShader(this->id);

      GLint success;
      glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);

      if (success == 0) {
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetShaderInfoLog(this->id, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "unable to get compilation errors for shader";
          return false;
        }
        this->compile_error.assign(info_log.data(), len);
      }

      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to compile shader";
        return false;
      }

      return this->valid = true;
    }

    auto Shader::shader_id() const noexcept -> ShaderID
    {
      return this->id;
    }

    auto Shader::error() const noexcept -> const std::string&
    {
      return this->compile_error;
    }

    auto Shader::is_valid() const noexcept -> bool
    {
      return this->valid;
    }

    auto Shader::get_source() const noexcept -> const std::string&
    {
      return this->source;
    }

    Program::Program()
     : valid(false)
    {
      this->id = glCreateProgram();
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to create new shader (gl error != GL_NO_ERROR)";
      }
    }

    Program::~Program()
    {
      if (this->id != 0) {
        glDeleteProgram(this->id);
      }
    }

    auto Program::attach(const Shader& shader) -> bool
    {
      if (this->id == 0) {
        LOG(ERROR) << "unable to link program (program id == 0)";
        return false;
      }

      glAttachShader(this->id, shader.shader_id());
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to attach shader";
        return false;
      }

      return true;
    }

    auto Program::link() -> bool
    {
      glLinkProgram(this->id);
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to link shader program";
        return false;
      }

      GLint success;
      glGetShaderiv(this->id, GL_LINK_STATUS, &success);

      if (success == 0) {
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetProgramInfoLog(this->id, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "unable to get compilation errors for shader";
          return false;
        }
        this->link_error.assign(info_log.data(), len);
      }

      return this->valid = true;
    }

    auto Program::program_id() const noexcept -> ProgramID
    {
      return this->id;
    }

    auto Program::error() const noexcept -> const std::string&
    {
      return this->link_error;
    }

    auto Program::is_valid() const noexcept -> bool
    {
      return this->valid;
    }

    auto Program::use() -> bool
    {
      glUseProgram(this->id);
      if (!GL_CHECK()) {
        LOG(ERROR) << "unable to use program";
        return false;
      }

      return true;
    }
  }  // namespace GL
}  // namespace ExpGame
