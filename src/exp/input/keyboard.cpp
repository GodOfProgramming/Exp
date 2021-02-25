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
        state.new_usertype<Keyboard>(
         Lua::Usertypes::Input::KEYBOARD, "instance", &Keyboard::instance, "check", &Keyboard::check, "changed", &Keyboard::changed);
      }
    }

    void Keyboard::on_event(KeyEvent e)
    {
      auto& pair  = this->key_map[e.key];
      pair.second = pair.first;
      pair.first  = e.action == Action::RELEASE ? Action::RELEASE : Action::PRESS;
    }

    auto Keyboard::check(Key k) -> Action
    {
      return this->key_map[k].first;
    }

    auto Keyboard::changed(Key k) -> bool
    {
      auto& pair = this->key_map[k];
      return pair.first != pair.second;
    }
  }  // namespace Input
}  // namespace Exp