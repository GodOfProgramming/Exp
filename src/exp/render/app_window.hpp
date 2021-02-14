#pragma once

#include "exp/input/ihandler.hpp"
#include "exp/settings/settings_manager.hpp"
#include "exp/util.hpp"

namespace Exp
{
  namespace Render
  {
    class AppWindow: public Input::IHandler
    {
      using WindowHandle = GLFWwindow*;

      AppWindow();

     public:
      AppWindow(const AppWindow&) = delete;
      AppWindow(AppWindow&&)      = delete;
      auto operator=(const AppWindow&) -> AppWindow& = delete;
      auto operator=(AppWindow&&) -> AppWindow& = delete;

      virtual ~AppWindow() = default;

      static auto instance() -> AppWindow&;

      /**
       * @brief Creates a window. If it cannot be crated the program terminates.
       */
      void create();

      void show();

      void hide();

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
  }  // namespace Render
}  // namespace Exp