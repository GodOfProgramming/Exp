#pragma once

namespace Exp
{
  namespace Game
  {
    enum class State
    {
      MAIN_MENU,
      LOADING,
      PLAYING,
      PAUSED,
    };

    struct Info
    {
      static auto instance() noexcept -> Info&;

      std::size_t player_id = 0;
      std::size_t fps       = 0;
      std::size_t frames    = 0;
      State state           = State::PLAYING;

      static void add_usertype(sol::state_view state);
    };
  }  // namespace Game

}  // namespace Exp