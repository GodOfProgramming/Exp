#pragma once

namespace Exp
{
  namespace Render
  {
    struct DrawDescription
    {
      bool wireframe = false;

      static void add_usertype(sol::state_view state);
    };
  }  // namespace Render
}  // namespace Exp
