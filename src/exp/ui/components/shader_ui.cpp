#include "shader_ui.hpp"

#include "exp/render/app_window.hpp"
#include "exp/resources/shaders.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      void ShaderUi::render()
      {
        using Render::AppWindow;
        auto& shaders = Resources::Shaders::instance();

        if (!this->initial_render) {
          auto& window = AppWindow::instance();
          auto size    = window.get_size();
          ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
          ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
          this->initial_render = true;
        }

        auto print_shader_info = [&](float indent, const char* shader_type, const Resources::ShaderMeta& meta) {
          ImGui::Indent(indent);
          ImGui::Text("%s: %s", shader_type, meta.file.c_str());
          {
            ImGui::Indent(indent);
            auto shader      = shaders.find_shader(meta.file)->second;
            auto* status_str = shader->is_valid() ? "Ok" : "Error";
            ImGui::Text("Status: %s", status_str);
            if (!shader->is_valid()) {
              ImGui::Text("%s", meta.error.c_str());
            }
            ImGui::Indent(-indent);
          }
          ImGui::Indent(-indent);
        };

        ImGui::Begin("Shaders");

        std::size_t id = 0;
        for (auto cfg_iter = shaders.cache_begin(); cfg_iter != shaders.cache_end(); cfg_iter++, id++) {
          auto& program_id  = cfg_iter->first;
          auto& shader_meta = cfg_iter->second;

          ImGui::Text("ID: \"%s\"", program_id.c_str());

          const auto program_iter = shaders.find_program(cfg_iter->first);

          bool loaded = program_iter != shaders.program_end();

          const char* status_txt = loaded ? "Yes" : "No";
          ImGui::Text("Loaded: %s", status_txt);

          if (loaded) {
            const auto& program = program_iter->second;

            auto& window = AppWindow::instance();
            auto dim     = window.get_size();
            auto indent  = dim.x * 0.025f;

            print_shader_info(indent, "Vertex", shader_meta.vertex);
            print_shader_info(indent, "Fragment", shader_meta.fragment);

            std::string text;
            if (shader_meta.uniforms.empty()) {
              text = "None";
            } else {
              text = Util::join(shader_meta.uniforms);
            }

            ImGui::Text("Uniforms: %s", text.c_str());

            if (!program->is_valid()) {
              ImGui::Text("Link Error: %s", shader_meta.link_error.c_str());
            }
          }

          ImGui::PushID(id);
          if (ImGui::Button("Reload")) {
            shaders.reload_program(program_id);
          }
          ImGui::PopID();
          ImGui::Separator();
        }

        ImGui::End();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp