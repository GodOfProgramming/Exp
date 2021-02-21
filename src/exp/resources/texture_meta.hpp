#pragma once

#include "exp/gl/enums.hpp"
#include "exp/gl/texture.hpp"
#include "exp/gl/texture_descriptor.hpp"

namespace Exp
{
  namespace R
  {
    struct TextureMeta
    {
      std::string file;
      GL::TextureDescriptor descriptor;
      std::shared_ptr<cimg_library::CImg<uint8_t>> img;
      std::shared_ptr<GL::Texture> tex;

      static auto has_file(const nlohmann::json& json, std::string& value) -> bool;
    };
  }  // namespace R
}  // namespace Exp
