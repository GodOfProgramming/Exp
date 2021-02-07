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

    class Shader
    {
     public:
      enum class Type
      {
        VERTEX   = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
      };

      using ShaderID = GLuint;

      Shader() noexcept;

      ~Shader() noexcept;

      auto compile(Type t, const std::string_view src) noexcept -> bool;

      auto shader_id() const noexcept -> ShaderID;

      auto error() const noexcept -> const std::string&;

      auto is_valid() const noexcept -> bool;

      auto get_source() const noexcept -> const std::string&;

     private:
      ShaderID id;
      std::string compile_error;
      bool valid;
      std::string source;
    };

    class Program
    {
     public:
      using ProgramID = GLuint;
      Program();
      ~Program();

      auto attach(const Shader& shaders) -> bool;

      auto link() -> bool;

      auto program_id() const noexcept -> ProgramID;

      auto error() const noexcept -> const std::string&;

      auto is_valid() const noexcept -> bool;

      auto use() -> bool;

     private:
      ProgramID id;
      std::string link_error;
      bool valid;
    };
  }  // namespace GL
}  // namespace ExpGame
