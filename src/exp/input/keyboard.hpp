#pragma once

#include "enums.hpp"
#include "key_event.hpp"

namespace Exp
{
  namespace Input
  {
    class Keyboard
    {
      Keyboard() = default;

      struct KeyState
      {
        Action curr                  = Action::RELEASE;
        Action last                  = Action::RELEASE;
        bool was_presssed_this_frame = false;
      };

     public:
      Keyboard(const Keyboard&) = delete;
      Keyboard(Keyboard&&)      = delete;
      auto operator=(const Keyboard&) -> Keyboard& = delete;
      auto operator=(Keyboard&&) -> Keyboard& = delete;

      static auto instance() noexcept -> Keyboard&;

      static void add_usertype(sol::state_view state);

      void update();

      void on_event(KeyEvent e);

      auto check(Key k) -> Action;

      auto changed(Key k) -> bool;

     private:
      std::map<Key, KeyState> key_map;
    };
  }  // namespace Input
}  // namespace Exp