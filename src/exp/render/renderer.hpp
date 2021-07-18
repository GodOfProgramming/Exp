#pragma once

#include "app_window.hpp"
#include "exp/game/world.hpp"
#include "exp/ui/ui_manager.hpp"

namespace Exp
{
  namespace Render
  {
    class Renderer
    {
      using UiManager = Ui::UiManager;
      using Window    = AppWindow;
      using World     = Game::World;

     public:
      Renderer() = default;

      auto init() -> bool;

      void render(World& world);

     private:
    };
  }  // namespace Render
}  // namespace Exp