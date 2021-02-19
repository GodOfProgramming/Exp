#include "gl_errors_ui.hpp"

#include "exp/gl/error_map.hpp"
#include "exp/render/app_window.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      void GlErrorsUi::render()
      {
        using Render::AppWindow;
        if (!this->initial_render) {
          auto& window = AppWindow::instance();
          auto size    = window.get_size();
          ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
          ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
          this->initial_render = true;
        }

        auto& errors = GL::ErrorMap::instance();

        bool is_open = this->is_enabled();
        ImGui::Begin("OpenGL Errors", &is_open);

        if (errors.error_count() == 0) {
          ImGui::Text("No Errors Detected");
        } else {
          auto& window = AppWindow::instance();
          auto dim     = window.get_size();
          auto indent  = dim.x * 0.025f;

          ImGuiID first = 1;
          for (const auto& kvp : errors) {
            auto id     = kvp.first;
            auto& error = kvp.second;

            ImGui::Text("Code: %u", id);
            ImGui::Text("Desc: %s", error.desc.c_str());
            if (ImGui::BeginChild(first++, { 0, dim.x * 0.2f }, true)) {
              for (const auto& file_lines_pair : error.occurrences) {
                auto& file  = file_lines_pair.first;
                auto& lines = file_lines_pair.second;
                ImGui::Text("File %s", file.c_str());
                std::vector<std::string> line_info(lines.size());
                for (const auto& line : lines) {
                  std::stringstream ss;
                  ss << line.first << ": " << line.second;
                  line_info.push_back(ss.str());
                }

                auto str = Util::join(line_info);
                ImGui::Indent(indent);
                ImGui::Text("%s", str.c_str());
                ImGui::Indent(-indent);
              }
            }
            ImGui::EndChild();
          }
        }

        ImGui::End();

        this->enable(is_open);
      }

      auto GlErrorsUi::display_text() noexcept -> std::string
      {
        return "Debug GL Errors";
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp