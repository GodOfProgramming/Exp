#pragma once

#include "enums.hpp"
#include "exp/constants.hpp"

namespace Exp
{
  namespace Input
  {
    class Mouse
    {
      Mouse() = default;

     public:
      Mouse(const Mouse&) = delete;
      Mouse(Mouse&&)      = delete;
      auto operator=(const Mouse&) -> Mouse& = delete;
      auto operator=(Mouse&&) -> Mouse& = delete;

      static auto instance() noexcept -> Mouse&;

      static void add_usertype(sol::state_view state);

     private:
    };
  }  // namespace Input
}  // namespace Exp