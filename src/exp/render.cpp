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

    void Renderer::render_to(Window& window, std::vector<std::shared_ptr<Game::Object>>& objects)
    {
      using Resources::Shaders;
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      GL_CHECK();
      glClear(GL_COLOR_BUFFER_BIT);
      GL_CHECK();

      auto& shaders = Shaders::instance();

      for (const auto object : objects) { object->meta.vbo->bind(); }

      this->ui.render();

      window.swap_buffers();
    }
  }  // namespace Render
}  // namespace ExpGame
