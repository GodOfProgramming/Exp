#include "ihandler.hpp"

namespace Exp
{
  namespace Input
  {
    IHandler::IHandler()
     : next(nullptr)
    {}

    void IHandler::set_next(IHandler* handler)
    {
      this->next = handler;
    }

    auto IHandler::get_next() -> IHandler*
    {
      return this->next;
    }
  }  // namespace Input
}  // namespace Exp
