#pragma once

#include "app_window.hpp"
#include "exp/game/object.hpp"
#include "exp/ui/ui_manager.hpp"

namespace Exp
{
  namespace Render
  {
    class Renderer
    {
      using UiManager = Ui::UiManager;
      using Window    = AppWindow;

     public:
      Renderer(UiManager& ui);

      void render_to(Window& window, std::vector<std::shared_ptr<Game::Object>>& objects);

     private:
      UiManager& ui;
    };
  }  // namespace Render
}  // namespace Exp