#pragma once

#include "exp/gl/enums.hpp"

namespace Exp
{
  namespace R
  {
    struct TextureDescription
    {
      struct
      {
        GL::TextureWrap s;
        GL::TextureWrap t;
      } wrap;
    };
    struct TextureMeta
    {
      cimg_library::CImg<uint8_t> tex;
    };
  }  // namespace R
}  // namespace Exp
