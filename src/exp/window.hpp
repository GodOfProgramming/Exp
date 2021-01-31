#pragma once

#include "input.hpp"
#include "settings.hpp"
#include "util.hpp"

namespace ExpGame
{
  namespace Window
  {
    class Window: public Input::IHandler
    {
      using WindowHandle = GLFWwindow*;

      Window();

     public:
      Window(const Window&) = delete;
      Window(Window&&)      = delete;
      auto operator=(const Window&) -> Window& = delete;
      auto operator=(Window&&) -> Window& = delete;

      virtual ~Window() = default;

      static auto instance() -> Window&;

      /**
       * @brief Creates a window. If it cannot be crated the program terminates.
       */
      void create();

      void swap_buffers();

      void poll_events();

      /**
       * @brief Close the window, can be called anytime
       */
      void close();

      /**
       * @brief Destroy the window, must be called after dependency cleanup
       */
      void destroy();

      void on_close(std::function<void(void)> f);

      auto get_size() -> glm::ivec2;

      auto handle(Input::KeyEvent e) -> Input::IHandler* override;
      auto handle(Input::MouseButtonEvent e) -> Input::IHandler* override;
      auto handle(Input::MouseMoveEvent e) -> Input::IHandler* override;

      auto operator*() -> WindowHandle;

     private:
      WindowHandle window;
      std::function<void(void)> on_close_callback;
    };
  }  // namespace Window
}  // namespace ExpGame