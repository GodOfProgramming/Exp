#pragma once

#include "util.hpp"

#define GL_CHECK() ExpGame::GL::ErrorMap::instance().check(__FILE__, __LINE__)

namespace ExpGame
{
  namespace GL
  {
    struct DrawDescription
    {
      bool wireframe = false;

      struct
      {
        bool front = true;
        bool back  = false;
      } polygon_mode;
    };

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

      auto error_count() const noexcept -> std::size_t;

     private:
      Internal errors;
      const std::map<GLenum, std::string> definitions;
    };

    enum class GlDraw
    {
      STREAM  = GL_STREAM_DRAW,
      STATIC  = GL_STATIC_DRAW,
      DYNAMIC = GL_DYNAMIC_DRAW,
    };

    class VBO
    {
     public:
      VBO();
      ~VBO();

      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto del() noexcept -> bool;

      auto is_valid() const noexcept -> bool;

     private:
      GLuint id;
    };

    class EBO
    {
     public:
      EBO();
      ~EBO();

      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto del() noexcept -> bool;

      auto is_valid() const noexcept -> bool;

     private:
      GLuint id;
    };

    class VAO
    {
     public:
      VAO(std::shared_ptr<VBO> vbo, std::shared_ptr<EBO> ebo);
      ~VAO();

      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto del() noexcept -> bool;

      template <
       GlDraw draw_type_t,
       typename vertex_type_t,
       typename index_type_t,
       typename datatype_t                          = typename vertex_type_t::value_type,
       vertex_type_t::length_type items_in_stride_t = vertex_type_t::length()>
      auto set(const std::vector<vertex_type_t>& data, const std::vector<index_type_t>& indices) noexcept -> bool;

      auto draw(const DrawDescription& desc) const noexcept -> bool;

      auto is_valid() const noexcept -> bool;

     private:
      std::shared_ptr<VBO> vbo;
      std::shared_ptr<EBO> ebo;
      GLuint id;
      bool valid;

      std::size_t index_count;
    };

    template <GlDraw draw_type_t, typename vertex_type_t, typename index_type_t, typename datatype_t, vertex_type_t::length_type items_in_stride_t>
    auto VAO::set(const std::vector<vertex_type_t>& data, const std::vector<index_type_t>& indices) noexcept -> bool
    {
      this->valid = false;

      if (!this->vbo->is_valid()) {
        LOG(WARNING) << "could not set vbo/ebo data, vbo not valid";
        return false;
      }

      if (!this->ebo->is_valid()) {
        LOG(WARNING) << "could not set vbo/ebo data, ebo not valid";
        return false;
      }

      if (!this->bind()) {
        LOG(WARNING) << "could not set vbo/ebo data, not able to bind vao";
        return false;
      }

      if (!this->vbo->bind()) {
        LOG(WARNING) << "could not set vbo data, not able to bind vbo";
        return false;
      }

      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_type_t) * data.size(), data.data(), static_cast<GLenum>(draw_type_t));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vbo data, not able to set buffer data";
        return false;
      }

      if (!this->ebo->bind()) {
        LOG(WARNING) << "could not set ebo data, not able to bind ebo";
        return false;
      }

      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_type_t) * indices.size(), indices.data(), static_cast<GLenum>(draw_type_t));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set ebo data, not able to set buffer data";
        return false;
      }

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, items_in_stride_t * sizeof(datatype_t), nullptr);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vbo data, not able to enable vertex attrib pointer";
        return false;
      }

      glEnableVertexAttribArray(0);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vao data, unable to enable attrib array";
        return false;
      }

      this->index_count = indices.size();

      return this->valid = true;
    }

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

      auto compile(Type t, const std::string& src, std::string& errstr) noexcept -> bool;

      auto shader_id() const noexcept -> ShaderID;

      auto is_valid() const noexcept -> bool;

     private:
      bool valid;
      ShaderID id;
    };

    class Program
    {
     public:
      using ProgramID = GLuint;
      Program();
      ~Program();

      auto attach(const Shader& shaders) -> bool;

      auto link(std::string& errstr) -> bool;

      auto program_id() const noexcept -> ProgramID;

      auto is_valid() const noexcept -> bool;

      auto use() -> bool;

     private:
      ProgramID id;
      bool valid;
    };

    class Uniform
    {
     public:
      Uniform(const std::string_view name);
      void set_float(float f);
      void set_vec2(float x, float y);
      void set_vec3(float x, float y, float z);
      void set_vec4(float x, float y, float z, float w);

      auto enable(const Program& shader) -> bool;

     private:
      const std::string_view name;
      std::tuple<float, glm::vec2, glm::vec3, glm::vec4> value;
    };
  }  // namespace GL
}  // namespace ExpGame
