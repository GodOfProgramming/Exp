#include "input.hpp"

namespace ExpGame
{
  auto Input::instance() -> Input&
  {
    static Input input;
    return input;
  }

  Input::Input()
   : root_handler(nullptr)
  {}

  void Input::process(KeyEvent event)
  {
    auto* curr = this->root_handler;
    do {
      curr = curr->handle(event);
    } while (curr != nullptr);
  }

  void Input::process(MouseButtonEvent event)
  {
    auto* curr = this->root_handler;
    do {
      curr = curr->handle(event);
    } while (curr != nullptr);
  }

  void Input::process(MouseMoveEvent event)
  {
    auto* curr = this->root_handler;
    do {
      curr = curr->handle(event);
    } while (curr != nullptr);
  }

  void Input::set_root_handler(IHandler* handler)
  {
    this->root_handler = handler;
  }

  Input::IHandler::IHandler()
   : next(nullptr)
  {}

  void Input::IHandler::set_next(IHandler* handler)
  {
    this->next = handler;
  }

  auto Input::IHandler::get_next() -> IHandler*
  {
    return this->next;
  }
}  // namespace ExpGame
