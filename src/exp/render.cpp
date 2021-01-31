#include "render.hpp"

namespace ExpGame
{
  void Renderer::render_to(Window& window)
  {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    window.swap_buffers();
  }
}  // namespace ExpGame
