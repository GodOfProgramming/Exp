#include "debug_ui.hpp"

#include "exp/render/app_window.hpp"
#include "gl_errors_ui.hpp"
#include "shader_ui.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      void DebugUi::render()
      {
        using Render::AppWindow;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("Debug")) {
            if (ImGui::MenuItem("Demo Window")) {
              this->show_demo_window = !this->show_demo_window;
            }

            if (ImGui::MenuItem("Shaders")) {
              this->show_shader_debug = !this->show_shader_debug;
            }

            if (ImGui::MenuItem("OpenGL Errors")) {
              this->show_gl_errors = !this->show_gl_errors;
            }

            if (ImGui::MenuItem("Exit")) {
              auto& window = AppWindow::instance();
              window.close();
            }
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }

        if (this->show_demo_window) {
          ImGui::ShowDemoWindow();
        }

        if (this->show_shader_debug) {
          static ShaderUi ui;
          ui.render();
        }

        if (this->show_gl_errors) {
          static GlErrorsUi ui;
          ui.render();
        }
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp