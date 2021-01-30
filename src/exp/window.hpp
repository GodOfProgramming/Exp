#pragma once

#include "settings.hpp"
#include "util.hpp"

namespace ExpGame
{
  class Window
  {
    using WindowHandle = GLFWwindow*;
    using CreateResult = Result<Void, std::string>;

    Window();

   public:
    Window(const Window&) = delete;
    Window(Window&&)      = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window();

    static auto instance() -> Window&;

    auto create() -> CreateResult;

   private:
    WindowHandle window;
  };
}  // namespace ExpGame