#pragma once

namespace Exp
{
  namespace GL
  {
    struct DrawDescription
    {
      bool wireframe = false;

      static void add_usertype(sol::state_view& state);
    };
  }  // namespace GL
}  // namespace Exp
