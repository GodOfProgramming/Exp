#include "vbo.hpp"

#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    VBO::VBO()
     : id(0)
    {}

    VBO::~VBO()
    {
      if (this->is_valid()) {
        std::size_t attempts = 0;
        while (!this->del() && attempts < 5) { attempts++; }
      }
    }

    auto VBO::gen() noexcept -> bool
    {
      glGenBuffers(1, &this->id);
      return GL_CHECK();
    }

    auto VBO::bind() const noexcept -> bool
    {
      if (!this->is_valid()) {
        LOG(WARNING) << "attempted to bind vbo without validity";
        return false;
      }
      glBindBuffer(GL_ARRAY_BUFFER, this->id);
      return GL_CHECK();
    }

    auto VBO::del() noexcept -> bool
    {
      glDeleteBuffers(1, &this->id);
      if (!GL_CHECK()) {
        return false;
      }
      this->id = 0;
      return true;
    }

    auto VBO::is_valid() const noexcept -> bool
    {
      return this->id != 0;
    }
  }  // namespace GL
}  // namespace Exp