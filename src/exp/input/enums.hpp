#pragma once

namespace Exp
{
  namespace Input
  {
    enum class Key
    {
      ESC = GLFW_KEY_ESCAPE,
      F4  = GLFW_KEY_F4,
    };

    enum class Action
    {
      Press   = GLFW_PRESS,
      Release = GLFW_RELEASE,
    };
  }  // namespace Input
}  // namespace Exp