#pragma once

#include "ihandler.hpp"
#include "key_event.hpp"
#include "mouse_button_event.hpp"
#include "mouse_move_event.hpp"

namespace Exp
{
  namespace Input
  {
    class Dispatcher
    {
      Dispatcher();

     public:
      Dispatcher(const Dispatcher&) = delete;
      Dispatcher(Dispatcher&&)      = delete;
      auto operator=(const Dispatcher&) -> Dispatcher& = delete;
      auto operator=(Dispatcher&&) -> Dispatcher& = delete;

      static auto instance() -> Dispatcher&;

      void set_root_handler(IHandler* handler);

      void process(KeyEvent event);
      void process(MouseButtonEvent event);
      void process(MouseMoveEvent event);

     private:
      IHandler* root_handler;
    };
  }  // namespace Input
}  // namespace Exp