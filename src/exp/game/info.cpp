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
      state.new_usertype<Info>(Lua::Usertypes::INFO, "fps", &Info::fps, "frames", &Info::frames);
      state.set(Lua::Globals::GAME_INFO, &Info::instance());
    }
  }  // namespace Game
}  // namespace Exp