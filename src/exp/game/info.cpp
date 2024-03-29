#include "info.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Game
  {
    auto Info::instance() noexcept -> Info&
    {
      static Info info;
      return info;
    }

    void Info::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Game::INFO].get_type() == sol::type::none) {
        state.new_enum(
         Lua::Usertypes::Game::STATE, "MAIN_MENU", State::MAIN_MENU, "LOADING", State::LOADING, "PLAYING", State::PLAYING, "PAUSED", State::PAUSED);
      }

      if (state[Lua::Usertypes::Game::INFO].get_type() == sol::type::none) {
        state.new_usertype<Info>(
         Lua::Usertypes::Game::INFO,
         "instance",
         &Info::instance,
         "player_id",
         &Info::player_id,
         "fps",
         &Info::fps,
         "frames",
         &Info::frames,
         "state",
         &Info::state);
      }
    }
  }  // namespace Game
}  // namespace Exp