#include "render.hpp"

#include "game.hpp"
#include "resources.hpp"

namespace ExpGame
{
  namespace Render
  {
    Renderer::Renderer(UiManager& u)
     : ui(u)
    {}

    void Renderer::render_to(Window& window)
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      GL_CHECK();
      glClear(GL_COLOR_BUFFER_BIT);
      GL_CHECK();

      this->ui.render();

      window.swap_buffers();
    }
  }  // namespace Render
}  // namespace ExpGame
