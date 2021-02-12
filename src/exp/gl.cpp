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

    ErrorMap::ErrorMap()
    {
      this->errors[GL_NO_ERROR]                      = { "No error detected", {} };
      this->errors[GL_INVALID_ENUM]                  = { "Illegal enumeration", {} };
      this->errors[GL_INVALID_VALUE]                 = { "Illegal value parameter", {} };
      this->errors[GL_INVALID_OPERATION]             = { "Wrong parameter for state change", {} };
      this->errors[GL_STACK_OVERFLOW]                = { "Stack push overflow", {} };
      this->errors[GL_STACK_UNDERFLOW]               = { "Stack pop on empty stack", {} };
      this->errors[GL_OUT_OF_MEMORY]                 = { "Not enough memory for given operation", {} };
      this->errors[GL_INVALID_FRAMEBUFFER_OPERATION] = { "Read/write to framebuffer that is not complete", {} };
    }

    auto ErrorMap::check(const char* file, int line) noexcept -> bool
    {
      auto err = glGetError();
      if (err == GL_NO_ERROR) {
        return true;
      }

      auto entry = this->errors.find(err);
      if (entry == this->errors.end()) {
        this->errors[err].occurrences[std::string(file)][line]++;
      } else {
        auto& entry = this->errors[err];
        entry.desc  = std::to_string(err);
        entry.occurrences[std::string(file)][line]++;
      }
      return false;
    }

    auto ErrorMap::begin() const noexcept -> Iter
    {
      return this->errors.begin();
    }

    auto ErrorMap::end() const noexcept -> Iter
    {
      return this->errors.end();
    }

    VAO::~VAO()
    {
      DLOG(INFO) << "deleted vao";
      if (this->valid()) {
        this->del();
      }
    }

    auto VAO::gen() noexcept -> bool
    {
      glGenVertexArrays(1, &this->id);
      return GL_CHECK();
    }

    auto VAO::bind() const noexcept -> bool
    {
      glBindVertexArray(this->id);
      return GL_CHECK();
    }

    auto VAO::del() noexcept -> bool
    {
      glDeleteVertexArrays(1, &this->id);
      this->id = 0;
      return GL_CHECK();
    }

    auto VAO::valid() const noexcept -> bool
    {
      return this->id != 0;
    }

    VBO::VBO(std::shared_ptr<VAO> v)
     : vao(v)
    {}

    VBO::~VBO()
    {
      DLOG(INFO) << "deleted vbo";
      if (this->valid()) {
        this->del();
      }
    }

    auto VBO::gen() noexcept -> bool
    {
      glGenBuffers(1, &this->id);
      return GL_CHECK();
    }

    auto VBO::bind() const noexcept -> bool
    {
      glBindBuffer(GL_ARRAY_BUFFER, this->id);
      return GL_CHECK();
    }

    auto VBO::del() noexcept -> bool
    {
      glDeleteBuffers(1, &this->id);
      this->id = 0;
      return GL_CHECK();
    }

    auto VBO::valid() const noexcept -> bool
    {
      return this->id != 0;
    }

    Shader::Shader() noexcept
     : valid(false)
     , id(0)
    {}

    Shader::~Shader() noexcept
    {
      if (this->id != 0) {
        glDeleteShader(this->id);
        GL_CHECK();
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

      if (success == 0) {
        result      = false;
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetShaderInfoLog(this->id, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "unable to get compilation errors for shader";
          result = false;
        }
        this->compile_error.assign(info_log.data(), static_cast<std::size_t>(len));
      }

      return this->valid = result;
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
        GL_CHECK();
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
      glGetProgramiv(this->id, GL_LINK_STATUS, &success);
      GL_CHECK();

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
      return GL_CHECK();
    }
  }  // namespace GL
}  // namespace ExpGame
