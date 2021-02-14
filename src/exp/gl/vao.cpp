#include "vao.hpp"

namespace Exp
{
  namespace GL
  {
    VAO::VAO(std::shared_ptr<VBO> v, std::shared_ptr<EBO> e)
     : vbo(v)
     , ebo(e)
     , id(0)
     , valid(false)
     , index_count(0)
    {}

    VAO::~VAO()
    {
      if (this->is_valid()) {
        std::size_t attempts = 0;
        while (!this->del() && attempts < 5) { attempts++; }
      }
    }

    auto VAO::gen() noexcept -> bool
    {
      glGenVertexArrays(1, &this->id);
      return GL_CHECK();
    }

    auto VAO::bind() const noexcept -> bool
    {
      if (this->id == 0) {
        LOG(WARNING) << "attempted to bind vao without validity";
        return false;
      }
      glBindVertexArray(this->id);
      return GL_CHECK();
    }

    auto VAO::del() noexcept -> bool
    {
      glDeleteVertexArrays(1, &this->id);
      if (!GL_CHECK()) {
        return false;
      }
      this->id    = 0;
      this->valid = false;
      return true;
    }

    auto VAO::draw(const DrawDescription& desc) const noexcept -> bool
    {
      if (!this->bind()) {
        LOG(WARNING) << "could not draw object, cannot bind vao";
      }

      if (desc.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        if (!GL_CHECK()) {
          LOG(WARNING) << "cannot draw object, cannot set writeframe";
          return false;
        }
      } else {
        if (desc.polygon_mode.front && desc.polygon_mode.back) {
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          if (!GL_CHECK()) {
            LOG(WARNING) << "cannot draw object, cannot draw front and back";
            return false;
          }
        } else if (desc.polygon_mode.front) {
          glPolygonMode(GL_FRONT, GL_FILL);
          if (!GL_CHECK()) {
            LOG(WARNING) << "cannot draw object, cannot draw front";
            return false;
          }
        } else if (desc.polygon_mode.back) {
          glPolygonMode(GL_BACK, GL_FILL);
          if (!GL_CHECK()) {
            LOG(WARNING) << "cannot draw object, cannot draw front";
            return false;
          }
        }
      }

      glDrawElements(GL_TRIANGLES, this->index_count, GL_UNSIGNED_INT, 0);
      return GL_CHECK();
    }

    auto VAO::is_valid() const noexcept -> bool
    {
      return this->id != 0 && this->valid;
    }
  }  // namespace GL
}  // namespace Exp
