#include "draw_description.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace GL
  {
    void DrawDescription::add_usertype(sol::state_view state)
    {
      state.new_usertype<DrawDescription>(Lua::Usertypes::DRAW_DESCRIPTION, "wireframe", &DrawDescription::wireframe);
    }
  }  // namespace GL
}  // namespace Exp