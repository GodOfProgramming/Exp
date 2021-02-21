#pragma once

#include "texture_descriptor.hpp"

namespace Exp
{
  namespace GL
  {
    class Texture
    {
     public:
      auto gen() noexcept -> bool;

      auto bind() const noexcept -> bool;

      auto configure(const TextureDescriptor desc, std::shared_ptr<cimg_library::CImg<uint8_t>> img) noexcept -> bool;

      auto is_valid() const noexcept -> bool;

     private:
      GLuint id  = 0;
      bool valid = false;
    };
  }  // namespace GL
}  // namespace Exp