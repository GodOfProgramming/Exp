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

    void Keyboard::update()
    {
      for (auto& kvp : this->key_map) {
        auto& state = kvp.second;
        if (!state.was_presssed_this_frame) {
          state.last = state.curr;
        } else {
          state.was_presssed_this_frame = false;
        }
      }
    }

    void Keyboard::on_event(KeyEvent e)
    {
      auto& pair                   = this->key_map[e.key];
      pair.was_presssed_this_frame = true;
      pair.last                    = pair.curr;
      pair.curr                    = e.action == Action::RELEASE ? Action::RELEASE : Action::PRESS;
    }

    auto Keyboard::check(Key k) -> Action
    {
      return this->key_map[k].curr;
    }

    auto Keyboard::changed(Key k) -> bool
    {
      auto& pair = this->key_map[k];
      return pair.curr != pair.last;
    }
  }  // namespace Input
}  // namespace Exp