#include "render.hpp"

namespace ExpGame
{
  void Renderer::render_to(Window& window)
  {
    window.swap_buffers();
  }
}  // namespace ExpGame
