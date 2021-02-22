#pragma once

#include "animation_meta.hpp"
#include "iresource.hpp"

namespace Exp
{
  namespace R
  {
    class Animations: public IResource
    {
      using AnimationMap = std::map<std::string, std::shared_ptr<AnimationMeta>>;
      Animations()       = default;

     public:
      Animations(const Animations&) = delete;
      Animations(Animations&&)      = delete;
      auto operator=(const Animations&) -> Animations& = delete;
      auto operator=(Animations&&) -> Animations& = delete;

      static auto instance() noexcept -> Animations&;

      void load_all() final;
      void release() final;

      auto find(std::string id) const noexcept -> AnimationMap::const_iterator;
      auto begin() const noexcept -> AnimationMap::const_iterator;
      auto end() const noexcept -> AnimationMap::const_iterator;

     private:
      AnimationMap animation_map;
    };
  }  // namespace R
}  // namespace Exp