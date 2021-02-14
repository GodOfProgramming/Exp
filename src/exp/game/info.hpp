#pragma once

namespace Exp
{
  namespace Game
  {
    struct Info
    {
      static auto instance() noexcept -> Info&;

      std::size_t fps    = 0;
      std::size_t frames = 0;
    };
  }  // namespace Game

}  // namespace Exp