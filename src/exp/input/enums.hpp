#pragma once

#include "exp/constants.hpp"

namespace Exp
{
  namespace Input
  {
    template <typename T>
    void add_usertype(sol::state_view);

    enum class MouseButton
    {
      MB1 = GLFW_MOUSE_BUTTON_1,
      MB2 = GLFW_MOUSE_BUTTON_2,
      MB3 = GLFW_MOUSE_BUTTON_3,
      MB4 = GLFW_MOUSE_BUTTON_4,
      MB5 = GLFW_MOUSE_BUTTON_5,
      MB6 = GLFW_MOUSE_BUTTON_6,
      MB7 = GLFW_MOUSE_BUTTON_7,
      MB8 = GLFW_MOUSE_BUTTON_8,

      // razer mamba
      LEFT   = MB1,
      RIGHT  = MB2,
      MIDDLE = MB3,
      EXTRA1 = MB4,
      EXTRA2 = MB5,
      EXTRA3 = MB6,
      EXTRA4 = MB7,
      EXTRA5 = MB8
    };

    template <>
    void add_usertype<MouseButton>(sol::state_view state);

    enum class Key
    {
      A = GLFW_KEY_A,
      B = GLFW_KEY_B,
      C = GLFW_KEY_C,
      D = GLFW_KEY_D,
      E = GLFW_KEY_E,
      F = GLFW_KEY_F,
      G = GLFW_KEY_G,
      H = GLFW_KEY_H,
      I = GLFW_KEY_I,
      J = GLFW_KEY_J,
      K = GLFW_KEY_K,
      L = GLFW_KEY_L,
      M = GLFW_KEY_M,
      N = GLFW_KEY_N,
      O = GLFW_KEY_O,
      P = GLFW_KEY_P,
      Q = GLFW_KEY_Q,
      R = GLFW_KEY_R,
      S = GLFW_KEY_S,
      T = GLFW_KEY_T,
      U = GLFW_KEY_U,
      V = GLFW_KEY_V,
      W = GLFW_KEY_W,
      X = GLFW_KEY_X,
      Y = GLFW_KEY_Y,
      Z = GLFW_KEY_Z,

      ZERO  = GLFW_KEY_0,
      ONE   = GLFW_KEY_1,
      TWO   = GLFW_KEY_2,
      THREE = GLFW_KEY_3,
      FOUR  = GLFW_KEY_4,
      FIVE  = GLFW_KEY_5,
      SIX   = GLFW_KEY_6,
      SEVEN = GLFW_KEY_7,
      EIGHT = GLFW_KEY_8,
      NINE  = GLFW_KEY_9,

      EQUALS = GLFW_KEY_EQUAL,
      MINUS  = GLFW_KEY_MINUS,

      SPACE     = GLFW_KEY_SPACE,
      GRAVE     = GLFW_KEY_GRAVE_ACCENT,
      SLASH     = GLFW_KEY_SLASH,
      BACKSLASH = GLFW_KEY_BACKSLASH,

      ESC = GLFW_KEY_ESCAPE,

      TAB  = GLFW_KEY_TAB,
      CAPS = GLFW_KEY_CAPS_LOCK,

      ENTER = GLFW_KEY_ENTER,

      F1  = GLFW_KEY_F1,
      F2  = GLFW_KEY_F2,
      F3  = GLFW_KEY_F3,
      F4  = GLFW_KEY_F4,
      F5  = GLFW_KEY_F5,
      F6  = GLFW_KEY_F6,
      F7  = GLFW_KEY_F7,
      F8  = GLFW_KEY_F8,
      F9  = GLFW_KEY_F9,
      F10 = GLFW_KEY_F10,
      F11 = GLFW_KEY_F11,
      F12 = GLFW_KEY_F12,

      UP    = GLFW_KEY_UP,
      DOWN  = GLFW_KEY_DOWN,
      LEFT  = GLFW_KEY_LEFT,
      RIGHT = GLFW_KEY_RIGHT,

      ALT_LEFT  = GLFW_KEY_LEFT_ALT,
      ALT_RIGHT = GLFW_KEY_RIGHT_ALT,

      CTRL_LEFT  = GLFW_KEY_LEFT_CONTROL,
      CTRL_RIGHT = GLFW_KEY_RIGHT_CONTROL,

      SHIFT_LEFT  = GLFW_KEY_LEFT_SHIFT,
      SHIFT_RIGHT = GLFW_KEY_RIGHT_SHIFT,

      HOME = GLFW_KEY_HOME,
      END  = GLFW_KEY_END,
    };

    template <>
    void add_usertype<Key>(sol::state_view state);

    enum class GamepadButton
    {
      BUTTON0,   // A
      BUTTON1,   // B
      BUTTON2,   // X
      BUTTON3,   // Y
      BUTTON4,   // LB
      BUTTON5,   // RB
      BUTTON6,   // Select
      BUTTON7,   // Start
      BUTTON8,   // Joy L
      BUTTON9,   // Joy R
      BUTTON10,  // D-Pad up
      BUTTON11,  // D-Pad right
      BUTTON12,  // D-Pad down
      BUTTON13,  // D-Pad left

      // xbox one
      A           = BUTTON0,
      B           = BUTTON1,
      X           = BUTTON2,
      Y           = BUTTON3,
      LB          = BUTTON4,
      RB          = BUTTON5,
      SELECT      = BUTTON6,
      START       = BUTTON7,
      STICK_LEFT  = BUTTON8,
      STICK_RIGHT = BUTTON9,
      DPAD_UP     = BUTTON10,
      DPAD_RIGHT  = BUTTON11,
      DPAD_DOWN   = BUTTON12,
      DPAD_LEFT   = BUTTON13
    };

    template <>
    void add_usertype<GamepadButton>(sol::state_view state);

    enum class GamepadAxis
    {
      AXIS0 = GLFW_GAMEPAD_AXIS_LEFT_X,
      AXIS1 = GLFW_GAMEPAD_AXIS_LEFT_Y,
      AXIS2 = GLFW_GAMEPAD_AXIS_RIGHT_X,
      AXIS3 = GLFW_GAMEPAD_AXIS_RIGHT_Y,
      AXIS4 = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
      AXIS5 = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,

      // xbox one
      STICK_LEFT_X  = AXIS0,
      STICK_LEFT_Y  = AXIS1,
      STICK_RIGHT_X = AXIS2,
      STICK_RIGHT_Y = AXIS3,
      TRIGGER_LEFT  = AXIS4,
      TRIGGER_RIGHT = AXIS5
    };

    template <>
    void add_usertype<GamepadAxis>(sol::state_view state);

    enum class Action
    {
      RELEASE = GLFW_RELEASE,
      PRESS   = GLFW_PRESS,
    };

    template <>
    void add_usertype<Action>(sol::state_view state);
  }  // namespace Input
}  // namespace Exp