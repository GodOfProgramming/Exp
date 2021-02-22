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
        state.new_usertype<Info>(Lua::Usertypes::Game::INFO, "instance", &Info::instance, "fps", &Info::fps, "frames", &Info::frames);
      }
    }
  }  // namespace Game
}  // namespace Exp