#pragma once

#include "ui.hpp"
#include "window.hpp"

namespace ExpGame
{
  class Renderer
  {
   public:
    Renderer(UiManager& ui);
    void render_to(Window& window);

   private:
    UiManager& ui;
  };
}  // namespace ExpGame
