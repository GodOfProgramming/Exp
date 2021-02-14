#include "info.hpp"

namespace Exp
{
  namespace Game
  {
    auto Info::instance() noexcept -> Info&
    {
      static Info info;
      return info;
    }
  }  // namespace Game
}  // namespace Exp