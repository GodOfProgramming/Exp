#include "renderer.hpp"

namespace Exp
{
  namespace Render
  {
    Renderer::Renderer() {}

    auto Renderer::init() -> bool
    {
      glEnable(GL_DEPTH_TEST);
      return GL_CHECK();
    }

    void Renderer::render(World& world)
    {
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      if (!GL_CHECK()) {
        LOG(ERROR) << "could not clear gl color";
        return;
      }

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      if (!GL_CHECK()) {
        LOG(ERROR) << "could not clear color buffer bit";
        return;
      }

      for (const auto object : world.entities()) {
        if (!object->meta.shader->use()) {
          LOG(WARNING) << "unable to draw game object, shader could not be used";
          continue;
        }

        object->prerender();

        if (!object->meta.model->vao->draw(object->meta.drawdesc)) {
          LOG(WARNING) << "unable to draw game object, vao not bound";
          continue;
        }
      }

      auto* ui = world.get_resource<UiManager>();

      ui->render();

      auto* window = world.get_resource<AppWindow>();

      window->swap_buffers();
    }
  }  // namespace Render
}  // namespace Exp