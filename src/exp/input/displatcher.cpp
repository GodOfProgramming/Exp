#include "dispatcher.hpp"

namespace Exp
{
  namespace Input
  {
    auto Dispatcher::instance() -> Dispatcher&
    {
      static Dispatcher input;
      return input;
    }

    Dispatcher::Dispatcher()
     : root_handler(nullptr)
    {}

    void Dispatcher::process(KeyEvent event)
    {
      auto* curr = this->root_handler;
      do {
        curr = curr->handle(event);
      } while (curr != nullptr);
    }

    void Dispatcher::process(MouseButtonEvent event)
    {
      auto* curr = this->root_handler;
      do {
        curr = curr->handle(event);
      } while (curr != nullptr);
    }

    void Dispatcher::process(MouseMoveEvent event)
    {
      auto* curr = this->root_handler;
      do {
        curr = curr->handle(event);
      } while (curr != nullptr);
    }

    void Dispatcher::set_root_handler(IHandler* handler)
    {
      this->root_handler = handler;
    }
  }  // namespace Input
}  // namespace Exp