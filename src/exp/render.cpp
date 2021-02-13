#include "render.hpp"

#include "game.hpp"

namespace ExpGame
{
  namespace Render
  {
    Renderer::Renderer(UiManager& u)
     : ui(u)
    {}

    void Renderer::render_to(Window& window, std::vector<std::shared_ptr<Game::Object>>& objects)
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      if (!GL_CHECK()) {
        LOG(ERROR) << "could not clear gl color";
        return;
      }

      glClear(GL_COLOR_BUFFER_BIT);
      if (!GL_CHECK()) {
        LOG(ERROR) << "could not clear color buffer bit";
        return;
      }

      for (const auto object : objects) {
        if (!object->meta.shader->use()) {
          LOG(WARNING) << "unable to draw game object, shader could not be used";
          continue;
        }

        if (!object->meta.model->vao->draw(object->meta.drawdesc)) {
          LOG(WARNING) << "unable to draw game object, vao not bound";
          continue;
        }
      }

      this->ui.render();

      window.swap_buffers();
    }
  }  // namespace Render
}  // namespace ExpGame
