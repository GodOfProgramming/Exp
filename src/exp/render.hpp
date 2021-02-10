#pragma once

#include "game.hpp"
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

      void render_to(Window& window, std::vector<std::shared_ptr<Game::Object>>& objects);

     private:
      UiManager& ui;
    };
  }  // namespace Render
}  // namespace ExpGame
