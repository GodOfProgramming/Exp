#pragma once

#include "ui.hpp"
#include "window.hpp"

namespace ExpGame
{
  namespace Render
  {
    class Renderer
    {
      using UiManager = Ui::UiManager;
      using Window    = Window::AppWindow;

     public:
      Renderer(UiManager& ui);
      void render_to(Window& window);

     private:
      UiManager& ui;
    };
  }  // namespace Render
}  // namespace ExpGame
