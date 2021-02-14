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

    void Info::add_usertype(sol::state& state)
    {
      state.new_usertype<Info>("Info", "fps", &Info::fps, "frames", &Info::frames);
    }
  }  // namespace Game
}  // namespace Exp