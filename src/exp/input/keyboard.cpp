#include "keyboard.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Input
  {
    auto Keyboard::instance() noexcept -> Keyboard&
    {
      static Keyboard keyboard;
      return keyboard;
    }

    void Keyboard::add_usertype(sol::state_view state)
    {
      if (state[Lua::Usertypes::Input::KEYBOARD].get_type() == sol::type::none) {
        state.new_usertype<Keyboard>(Lua::Usertypes::Input::KEYBOARD, "instance", &Keyboard::instance, "check", &Keyboard::check);
      }
    }

    void Keyboard::on_event(KeyEvent e)
    {
      this->key_map[e.key] = e.action == Action::RELEASE ? Action::RELEASE : Action::PRESS;
    }

    auto Keyboard::check(Key k) -> Action
    {
      return this->key_map[k];
    }
  }  // namespace Input
}  // namespace Exp