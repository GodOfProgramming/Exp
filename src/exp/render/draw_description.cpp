#include "draw_description.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Render
  {
    void DrawDescription::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Render::DRAW_DESCRIPTION].get_type() == sol::type::none) {
        state.new_usertype<DrawDescription>(Lua::Usertypes::Render::DRAW_DESCRIPTION, "wireframe", &DrawDescription::wireframe);
      }
    }
  }  // namespace Render
}  // namespace Exp