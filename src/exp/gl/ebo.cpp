#include "ebo.hpp"

#include "error_map.hpp"

namespace Exp
{
  namespace GL
  {
    EBO::EBO()
     : id(0)
    {}

    EBO::~EBO()
    {
      if (this->is_valid()) {
        std::size_t attempts = 0;
        while (!this->del() && attempts < 5) { attempts++; }
      }
    }

    auto EBO::gen() noexcept -> bool
    {
      glGenBuffers(1, &this->id);
      return GL_CHECK();
    }

    auto EBO::bind() const noexcept -> bool
    {
      if (!this->is_valid()) {
        LOG(WARNING) << "attempted to bind ebo without validity";
        return false;
      }
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
      return GL_CHECK();
    }

    auto EBO::del() noexcept -> bool
    {
      glDeleteBuffers(1, &this->id);
      if (!GL_CHECK()) {
        return false;
      }
      this->id = 0;
      return true;
    }

    auto EBO::is_valid() const noexcept -> bool
    {
      return this->id != 0;
    }
  }  // namespace GL
}  // namespace Exp
