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
      int width;
      int height;
      std::shared_ptr<uint8_t> img;
      std::shared_ptr<GL::Texture> tex;
      GL::TextureDescriptor descriptor;

      std::string file;

      static auto has_file(const nlohmann::json& json, std::string& value) -> bool;
    };
  }  // namespace R
}  // namespace Exp
