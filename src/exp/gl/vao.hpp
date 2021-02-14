#pragma once

#include "draw_description.hpp"
#include "ebo.hpp"
#include "enums.hpp"
#include "error_map.hpp"
#include "vbo.hpp"

namespace Exp
{
  namespace GL
  {
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
  }  // namespace GL
}  // namespace Exp
