#pragma once
#include "resources.hpp"

namespace ExpGame
{
  namespace Game
  {
    struct Info
    {
      static auto instance() noexcept -> Info&;

      std::size_t fps    = 0;
      std::size_t frames = 0;
    };

    class Object
    {
     public:
      Object(const Resources::ObjectMeta meta);

      const Resources::ObjectMeta meta;

     private:
    };
  }  // namespace Game
}  // namespace ExpGame
