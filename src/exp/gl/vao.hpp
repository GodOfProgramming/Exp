#pragma once

#include "ebo.hpp"
#include "enums.hpp"
#include "error_map.hpp"
#include "exp/render/draw_description.hpp"
#include "vbo.hpp"

namespace Exp
{
  namespace GL
  {
    class VAO
    {
      using DrawDescription = Render::DrawDescription;

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
       typename pos_t             = vertex_type_t::pos_t,
       typename norm_t            = vertex_type_t::norm_t,
       typename uv_t              = vertex_type_t::uv_t,
       typename pos_dt            = pos_t::value_type,
       typename norm_dt           = norm_t::value_type,
       typename uv_dt             = uv_t::value_type,
       typename pos_len_t         = pos_t::length_type,
       typename norm_len_t        = norm_t::length_type,
       typename uv_len_t          = uv_t::length_type,
       pos_len_t pos_stride_len   = pos_t::length(),
       norm_len_t norm_stride_len = norm_t::length(),
       uv_len_t uv_stride_len     = uv_t::length(),
       pos_len_t whole_len        = pos_stride_len + norm_stride_len + uv_stride_len>
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

    template <
     GlDraw draw_type_t,
     typename vertex_type_t,
     typename index_type_t,
     typename pos_t,
     typename norm_t,
     typename uv_t,
     typename pos_dt,
     typename norm_dt,
     typename uv_dt,
     typename pos_len_t,
     typename norm_len_t,
     typename uv_len_t,
     pos_len_t pos_stride_len,
     norm_len_t norm_stride_len,
     uv_len_t uv_stride_len,
     pos_len_t whole_len>
    auto VAO::set(const std::vector<vertex_type_t>& data, const std::vector<index_type_t>& indices) noexcept -> bool
    {
      static_assert(std::is_same<pos_dt, norm_dt>::value && std::is_same<norm_dt, uv_dt>::value);
      static_assert(std::is_same<pos_len_t, norm_len_t>::value && std::is_same<norm_len_t, uv_len_t>::value);

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

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, whole_len * sizeof(pos_dt), reinterpret_cast<void*>(0));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vbo data, not able to enable vertex attrib pointer (position)";
        return false;
      }

      glEnableVertexAttribArray(0);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vao data, unable to enable attrib array (position)";
        return false;
      }

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, whole_len * sizeof(norm_dt), reinterpret_cast<void*>(pos_stride_len * sizeof(pos_dt)));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vbo data, not able to enable vertex attrib pointer (normal)";
        return false;
      }

      glEnableVertexAttribArray(1);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vao data, unable to enable attrib array (normal)";
        return false;
      }

      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, whole_len * sizeof(uv_dt), reinterpret_cast<void*>((pos_stride_len + norm_stride_len) * sizeof(norm_dt)));
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vbo data, not able to enable vertex attrib pointer (uv)";
        return false;
      }

      glEnableVertexAttribArray(2);
      if (!GL_CHECK()) {
        LOG(WARNING) << "could not set vao data, unable to enable attrib array (uv)";
        return false;
      }

      this->index_count = indices.size();

      return this->valid = true;
    }
  }  // namespace GL
}  // namespace Exp
