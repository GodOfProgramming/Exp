#pragma once

#include <CImg.h>
namespace Exp
{
  namespace R
  {
    struct TextureMeta
    {
      std::shared_ptr<cimg_library::CImg<uint8_t>> tex;
    };
  }  // namespace R
}  // namespace Exp
