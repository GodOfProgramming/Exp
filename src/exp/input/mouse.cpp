#include "mouse.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Input
  {
    void Mouse::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Input::MOUSE].get_type() == sol::type::none) {
        state.new_usertype<Mouse>(Lua::Usertypes::Input::MOUSE);
      }
    }
  }  // namespace Input
}  // namespace Exp