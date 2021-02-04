#pragma once

#include "util.hpp"

#define GL_CHECK() ExpGame::GL::ErrorMap::instance().check(__FILE__, __LINE__)

namespace ExpGame
{
  namespace GL
  {
    struct Error
    {
      std::string desc;
      std::map<std::string, std::vector<int>> occurrences;
    };

    class ErrorMap
    {
      ErrorMap();

      using Internal = std::map<GLenum, Error>;
      using Iter     = Internal::iterator;

     public:
      ErrorMap(const ErrorMap&) = delete;
      ErrorMap(ErrorMap&&)      = delete;
      auto operator=(const ErrorMap&) -> ErrorMap& = delete;
      auto operator=(ErrorMap&&) -> ErrorMap& = delete;

      static auto instance() -> ErrorMap&;

      auto check(const char* file, int line) -> bool;

      auto begin() -> Iter;
      auto end() -> Iter;

     private:
      Internal errors;
    };

    enum class GlDraw
    {
      STREAM  = GL_STREAM_DRAW,
      STATIC  = GL_STATIC_DRAW,
      DYNAMIC = GL_DYNAMIC_DRAW,
    };

    class VBO
    {
      using VertexBuffer = std::vector<glm::vec3>;
      using Vertex       = VertexBuffer::value_type;

     public:
      auto gen() -> bool;

      auto bind() -> bool;

      template <GlDraw T>
      auto set(VertexBuffer& data) -> bool
      {
        if (!this->bind()) {
          return false;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), static_cast<GLenum>(T));
        return GL_CHECK();
      }

     private:
      GLuint id = 0;
    };

    enum class ShaderType
    {
      VERTEX   = GL_VERTEX_SHADER,
      FRAGMENT = GL_FRAGMENT_SHADER,
    };

    template <ShaderType T>
    class Shader
    {
     public:
      using ShaderID = GLuint;

      Shader()
      {
        this->id = glCreateShader(static_cast<GLenum>(T));

        if (!GL_CHECK()) {
          LOG(ERROR) << "Unable to create new shader (gl error != GL_NO_ERROR)";
        }
      }

      ~Shader()
      {
        if (this->id != 0) {
          glDeleteShader(this->id);
        }
      }

      auto compile(const std::string_view src) -> bool
      {
        if (this->id == 0) {
          LOG(ERROR) << "Unable to create new shader (shader = 0)";
          return false;
        }

        auto data = src.data();

        glShaderSource(this->id, 1, &data, src.length());
        if (!GL_CHECK()) {
          LOG(ERROR) << "Unable to set shader source";
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
            LOG(ERROR) << "Unable to get compilation errors for shader";
            return false;
          }
          this->compile_error.assign(info_log.data(), len);
        }

        if (!GL_CHECK()) {
          LOG(ERROR) << "Unable to compile shader";
          return false;
        }

        return true;
      }

      auto shader_id() -> ShaderID
      {
        return this->id;
      }

      auto error() -> const std::string&
      {
        return this->compile_error;
      }

     private:
      ShaderID id;
      std::string compile_error;
    };

    class Program
    {
     public:
      using ProgramID = GLuint;
      Program();
      ~Program();

      template <ShaderType T>
      auto attach(const Shader<T> shader) -> bool
      {
        if (this->id == 0) {
          LOG(ERROR) << "Unable to link program (program id == 0)";
          return false;
        }

        glAttachShader(this->id, shader.shader_id());
        if (!GL_CHECK()) {
          LOG(ERROR) << "Unable to attach shader";
          return false;
        }

        return true;
      }

      template <ShaderType T, class... S>
      auto attach(const Shader<T> shader, S&&... shaders) -> bool
      {
        return this->link(shader) && this->link(shaders...);
      }

      auto link() -> bool;

      auto program_id() -> ProgramID;

      auto use() -> bool;

      auto error() -> const std::string&;

     private:
      ProgramID id;
      std::string link_error;
    };
  }  // namespace GL
}  // namespace ExpGame
