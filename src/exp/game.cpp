#include "game.hpp"

namespace ExpGame
{
  namespace Game
  {
    auto Info::instance() noexcept -> Info&
    {
      static Info info;
      return info;
    }

    Object::Object(const Resources::ObjectMeta m)
     : meta(m)
    {}
  }  // namespace Game
}  // namespace ExpGame
