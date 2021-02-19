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

      static void add_usertype(sol::state_view& state);
    };
  }  // namespace Game

}  // namespace Exp