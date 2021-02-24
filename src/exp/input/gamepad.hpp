#pragma once

namespace Exp
{
  namespace Input
  {
    class Gamepad
    {
      Gamepad() = default;

     public:
      Gamepad(const Gamepad&) = delete;
      Gamepad(Gamepad&&)      = delete;
      auto operator=(const Gamepad&) -> Gamepad& = delete;
      auto operator=(Gamepad&&) -> Gamepad& = delete;

      static auto instance() noexcept -> Gamepad&;

      static void add_usertype(sol::state_view state);

     private:
    };
  }  // namespace Input
}  // namespace Exp