#pragma once

#include "iresource.hpp"
#include "texture_meta.hpp"

namespace Exp
{
  namespace R
  {
    class Textures: public IResource
    {
      using TextureMap = std::map<std::string, std::shared_ptr<TextureMeta>>;

     public:
      Textures()                = default;
      Textures(const Textures&) = delete;
      Textures(Textures&&)      = default;
      auto operator=(const Textures&) -> Textures& = delete;
      auto operator=(Textures&&) -> Textures& = default;

      void load_all(World& world) final;
      void release() final;

      auto find(std::string id) const noexcept -> TextureMap::const_iterator;
      auto begin() const noexcept -> TextureMap::const_iterator;
      auto end() const noexcept -> TextureMap::const_iterator;

     private:
      TextureMap textures;
    };
  }  // namespace R
}  // namespace Exp