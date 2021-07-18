#pragma once

#include "animation_meta.hpp"
#include "exp/game/world.hpp"
#include "iresource.hpp"

namespace Exp
{
  namespace R
  {
    class Animations: public IResource
    {
      using AnimationMap = std::map<std::string, std::shared_ptr<AnimationMeta>>;

     public:
      Animations()                  = default;
      Animations(const Animations&) = delete;
      Animations(Animations&&)      = default;
      auto operator=(const Animations&) -> Animations& = delete;
      auto operator=(Animations&&) -> Animations& = default;

      void load_all(World& world) final;
      void release() final;

      auto find(std::string id) const noexcept -> AnimationMap::const_iterator;
      auto begin() const noexcept -> AnimationMap::const_iterator;
      auto end() const noexcept -> AnimationMap::const_iterator;

     private:
      AnimationMap animation_map;
    };
  }  // namespace R
}  // namespace Exp