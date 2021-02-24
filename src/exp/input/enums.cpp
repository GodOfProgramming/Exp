#include "enums.hpp"

namespace Exp
{
  namespace Input
  {
    template <>
    void add_usertype<MouseButton>(sol::state_view state)
    {
#define STRINGIFY(b) #b, MouseButton::b
      if (state[Lua::Usertypes::Input::MOUSE_BUTTON].get_type() == sol::type::none) {
        state.new_enum(
         Lua::Usertypes::Input::MOUSE_BUTTON,
         STRINGIFY(MB1),
         STRINGIFY(MB2),
         STRINGIFY(MB3),
         STRINGIFY(MB4),
         STRINGIFY(MB5),
         STRINGIFY(MB6),
         STRINGIFY(MB7),
         STRINGIFY(MB8),
         STRINGIFY(LEFT),
         STRINGIFY(RIGHT),
         STRINGIFY(MIDDLE),
         STRINGIFY(EXTRA1),
         STRINGIFY(EXTRA2),
         STRINGIFY(EXTRA3),
         STRINGIFY(EXTRA4),
         STRINGIFY(EXTRA5));
      }
#undef STRINGIFY
    }

    template <>
    void add_usertype<Key>(sol::state_view state)
    {
#define STRINGIFY(e) #e, Key::e
      if (state[Lua::Usertypes::Input::KEY].get_type() == sol::type::none) {
        state.new_enum(
         Lua::Usertypes::Input::KEY,
         STRINGIFY(A),
         STRINGIFY(B),
         STRINGIFY(C),
         STRINGIFY(D),
         STRINGIFY(E),
         STRINGIFY(F),
         STRINGIFY(G),
         STRINGIFY(H),
         STRINGIFY(I),
         STRINGIFY(J),
         STRINGIFY(K),
         STRINGIFY(L),
         STRINGIFY(M),
         STRINGIFY(N),
         STRINGIFY(O),
         STRINGIFY(P),
         STRINGIFY(Q),
         STRINGIFY(R),
         STRINGIFY(S),
         STRINGIFY(T),
         STRINGIFY(U),
         STRINGIFY(V),
         STRINGIFY(W),
         STRINGIFY(X),
         STRINGIFY(Y),
         STRINGIFY(Z),
         STRINGIFY(ZERO),
         STRINGIFY(ONE),
         STRINGIFY(TWO),
         STRINGIFY(THREE),
         STRINGIFY(FOUR),
         STRINGIFY(FIVE),
         STRINGIFY(SIX),
         STRINGIFY(SEVEN),
         STRINGIFY(EIGHT),
         STRINGIFY(NINE),
         STRINGIFY(EQUALS),
         STRINGIFY(MINUS),
         STRINGIFY(SPACE),
         STRINGIFY(GRAVE),
         STRINGIFY(SLASH),
         STRINGIFY(BACKSLASH),
         STRINGIFY(ESC),
         STRINGIFY(TAB),
         STRINGIFY(CAPS),
         STRINGIFY(ENTER),
         STRINGIFY(F1),
         STRINGIFY(F2),
         STRINGIFY(F3),
         STRINGIFY(F4),
         STRINGIFY(F5),
         STRINGIFY(F6),
         STRINGIFY(F7),
         STRINGIFY(F8),
         STRINGIFY(F9),
         STRINGIFY(F10),
         STRINGIFY(F11),
         STRINGIFY(F12),
         STRINGIFY(UP),
         STRINGIFY(DOWN),
         STRINGIFY(LEFT),
         STRINGIFY(RIGHT),
         STRINGIFY(ALT_LEFT),
         STRINGIFY(ALT_RIGHT),
         STRINGIFY(CTRL_LEFT),
         STRINGIFY(CTRL_RIGHT),
         STRINGIFY(SHIFT_LEFT),
         STRINGIFY(SHIFT_RIGHT),
         STRINGIFY(HOME),
         STRINGIFY(END));
      }
#undef STRINGIFY
    }

    template <>
    void add_usertype<GamepadButton>(sol::state_view state)
    {
#define STRINGIFY(b) #b, GamepadButton::b
      if (state[Lua::Usertypes::Input::GAMEPAD_BUTTON].get_type() == sol::type::none) {
        state.new_enum(
         Lua::Usertypes::Input::GAMEPAD_BUTTON,
         STRINGIFY(BUTTON0),
         STRINGIFY(BUTTON1),
         STRINGIFY(BUTTON2),
         STRINGIFY(BUTTON3),
         STRINGIFY(BUTTON4),
         STRINGIFY(BUTTON5),
         STRINGIFY(BUTTON6),
         STRINGIFY(BUTTON7),
         STRINGIFY(BUTTON8),
         STRINGIFY(BUTTON9),
         STRINGIFY(BUTTON10),
         STRINGIFY(BUTTON11),
         STRINGIFY(BUTTON12),
         STRINGIFY(BUTTON13),
         STRINGIFY(A),
         STRINGIFY(B),
         STRINGIFY(X),
         STRINGIFY(Y),
         STRINGIFY(LB),
         STRINGIFY(RB),
         STRINGIFY(SELECT),
         STRINGIFY(START),
         STRINGIFY(STICK_LEFT),
         STRINGIFY(STICK_RIGHT),
         STRINGIFY(DPAD_UP),
         STRINGIFY(DPAD_RIGHT),
         STRINGIFY(DPAD_DOWN),
         STRINGIFY(DPAD_LEFT));
      }
#undef STRINGIFY
    }

    template <>
    void add_usertype<GamepadAxis>(sol::state_view state)
    {
#define STRINGIFY(a) #a, GamepadAxis::a
      if (state[Lua::Usertypes::Input::GAMEPAD_AXIS].get_type() == sol::type::none) {
        state.new_enum(
         Lua::Usertypes::Input::GAMEPAD_AXIS,
         STRINGIFY(AXIS0),
         STRINGIFY(AXIS1),
         STRINGIFY(AXIS2),
         STRINGIFY(AXIS3),
         STRINGIFY(AXIS4),
         STRINGIFY(AXIS5),
         STRINGIFY(STICK_LEFT_X),
         STRINGIFY(STICK_LEFT_Y),
         STRINGIFY(STICK_RIGHT_X),
         STRINGIFY(STICK_RIGHT_Y),
         STRINGIFY(TRIGGER_LEFT),
         STRINGIFY(TRIGGER_RIGHT));
      }
#undef STRINGIFY
    }

    template <>
    void add_usertype<Action>(sol::state_view state)
    {
#define STRINGIFY(a) #a, Action::a
      if (state[Lua::Usertypes::Input::ACTION].get_type() == sol::type::none) {
        state.new_enum(Lua::Usertypes::Input::ACTION, STRINGIFY(PRESS), STRINGIFY(RELEASE));
      }
#undef STRINGIFY
    }
  }  // namespace Input
}  // namespace Exp