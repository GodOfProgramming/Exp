#include "render.hpp"

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
      glClear(GL_COLOR_BUFFER_BIT);

      this->ui.render();

      window.swap_buffers();
    }
  }  // namespace Render
}  // namespace ExpGame
