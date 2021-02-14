#pragma once

namespace Exp
{
  namespace GL
  {
    struct DrawDescription
    {
      bool wireframe = false;

      struct
      {
        bool front = true;
        bool back  = false;
      } polygon_mode;
    };
  }  // namespace GL
}  // namespace Exp
