#include "draw_description.hpp"

namespace Exp
{
  namespace GL
  {
    void DrawDescription::add_usertype(sol::state_view state)
    {
      state.new_usertype<DrawDescription>("DrawDescription", "wireframe", &DrawDescription::wireframe);
    }
  }  // namespace GL
}  // namespace Exp