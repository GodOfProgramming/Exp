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
      std::map<std::string, std::map<int, std::size_t>> occurrences;
    };

    class ErrorMap
    {
      ErrorMap();

      using Internal = std::map<GLenum, Error>;
      using Iter     = Internal::const_iterator;

     public:
      ErrorMap(const ErrorMap&) = delete;
      ErrorMap(ErrorMap&&)      = delete;
      auto operator=(const ErrorMap&) -> ErrorMap& = delete;
      auto operator=(ErrorMap&&) -> ErrorMap& = delete;

      static auto instance() -> ErrorMap&;

      auto check(const char* file, int line) noexcept -> bool;

      auto begin() const noexcept -> Iter;
      auto end() const noexcept -> Iter;

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
      ~VBO();

      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto del() noexcept -> bool;

      template <GlDraw T>
      auto set(VertexBuffer& data) const noexcept -> bool
      {
        if (!this->bind()) {
          return false;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * data.size(), data.data(), static_cast<GLenum>(T));
        if (!GL_CHECK()) {
          return false;
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(glm::vec3), nullptr);
        if (!GL_CHECK()) {
          return false;
        }
        glEnableVertexAttribArray(0);
      }

      auto valid() const noexcept -> bool;

     private:
      GLuint id = 0;
    };

    class VAO
    {
     public:
      ~VAO();

     private:
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
      bool valid;
      ShaderID id;
      std::string compile_error;
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
