#pragma once

#include "key_event.hpp"
#include "mouse_button_event.hpp"
#include "mouse_move_event.hpp"

namespace Exp
{
  namespace Input
  {
    class IHandler
    {
     public:
      IHandler();
      virtual ~IHandler() = default;

      virtual auto handle(KeyEvent) -> IHandler*         = 0;
      virtual auto handle(MouseButtonEvent) -> IHandler* = 0;
      virtual auto handle(MouseMoveEvent) -> IHandler*   = 0;

      void set_next(IHandler* next);
      auto get_next() -> IHandler*;

     private:
      IHandler* next;
    };
  }  // namespace Input
}  // namespace Exp