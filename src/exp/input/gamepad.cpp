#include "gamepad.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Input
  {
    void Gamepad::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Input::GAMEPAD].get_type() == sol::type::none) {
        state.new_usertype<Gamepad>(Lua::Usertypes::Input::GAMEPAD);
      }
    }
  }  // namespace Input
}  // namespace Exp