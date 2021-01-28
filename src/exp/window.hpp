#pragma once

namespace ExpGame
{
  class Window
  {
    using WindowHandle = GLFWwindow*;

   public:
    Window();
    ~Window();

    auto create() -> bool;

   private:
    WindowHandle window;
  };
}  // namespace ExpGame