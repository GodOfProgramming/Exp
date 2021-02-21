#include "draw_description.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Render
  {
    void DrawDescription::add_usertype(sol::state_view state)
    {
      state.new_usertype<DrawDescription>(Lua::Usertypes::Render::DRAW_DESCRIPTION, "wireframe", &DrawDescription::wireframe);
    }
  }  // namespace Render
}  // namespace Exp