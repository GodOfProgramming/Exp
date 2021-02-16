#pragma once

#include "iresource.hpp"
#include "texture_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Textures: public IResource
    {
      Textures() = default;

      using TextureMap = std::map<std::string, std::shared_ptr<TextureMeta>>;

     public:
      Textures(const Textures&) = delete;
      Textures(Textures&&)      = delete;
      auto operator=(const Textures&) -> Textures& = delete;
      auto operator=(Textures&&) -> Textures& = delete;

      static auto instance() noexcept -> Textures&;

      void load_all() final;
      void release() final;

     private:
      TextureMap textures;
    };
  }  // namespace R
}  // namespace Exp