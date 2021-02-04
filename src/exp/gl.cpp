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
          entry->second.occurrences[std::string(file)].push_back(line);
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

    Program::Program()
    {
      this->id = glCreateProgram();
      if (!GL_CHECK()) {
        LOG(ERROR) << "Unable to create new shader (gl error != GL_NO_ERROR)";
      }
    }

    Program::~Program()
    {
      if (this->id != 0) {
        glDeleteProgram(this->id);
      }
    }

    auto Program::link() -> bool
    {
      glLinkProgram(this->id);
      if (!GL_CHECK()) {
        LOG(ERROR) << "Unable to link shader program";
        return false;
      }

      GLint success;
      glGetShaderiv(this->id, GL_LINK_STATUS, &success);

      if (success == 0) {
        GLsizei len = 0;
        std::array<char, 1024> info_log;
        glGetProgramInfoLog(this->id, info_log.size(), &len, info_log.data());
        if (!GL_CHECK()) {
          LOG(ERROR) << "Unable to get compilation errors for shader";
          return false;
        }
        this->link_error.assign(info_log.data(), len);
      }

      return true;
    }

    auto Program::program_id() -> ProgramID
    {
      return this->id;
    }

    auto Program::use() -> bool
    {
      glUseProgram(this->id);
      if (!GL_CHECK()) {
        LOG(ERROR) << "Unable to use program";
        return false;
      }

      return true;
    }

    auto Program::error() -> const std::string&
    {
      return this->link_error;
    }
  }  // namespace GL
}  // namespace ExpGame
