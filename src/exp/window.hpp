#pragma once

#include "settings.hpp"
#include "util.hpp"

namespace ExpGame
{
  class Window
  {
    using WindowHandle = GLFWwindow*;

    Window();

   public:
    Window(const Window&) = delete;
    Window(Window&&)      = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    ~Window();

    static auto instance() -> Window&;

    /**
     * @brief Creates a window. If it cannot be crated the program terminates.
     */
    void create();

    void swap_buffers();

    void poll_events();

    void on_close(std::function<void(void)> f);

   private:
    WindowHandle window;
    std::function<void(void)> on_close_callback;
  };
}  // namespace ExpGame