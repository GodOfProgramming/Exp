#pragma once

#include "enums.hpp"

namespace Exp
{
  namespace GL
  {
    struct TextureDescriptor
    {
      TextureWrap wrap_s = TextureWrap::REPEAT;
      TextureWrap wrap_t = TextureWrap::REPEAT;

      TextureFilter min = TextureFilter::LINEAR;
      TextureFilter mag = TextureFilter::LINEAR;
    };
  }  // namespace GL
}  // namespace Exp