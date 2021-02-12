#include "ui.hpp"

#include "constants.hpp"
#include "gl.hpp"
#include "io.hpp"
#include "resources.hpp"
#include "window.hpp"

namespace ExpGame
{
  namespace Ui
  {
    using IO::File;
    using Window::AppWindow;

    auto UiManager::instance() -> UiManager&
    {
      static UiManager manager;
      return manager;
    }

    UiManager::UiManager()
    {
      IMGUI_CHECKVERSION();
      this->context = ImGui::CreateContext();

      auto& window = AppWindow::instance();
      ImGui_ImplGlfw_InitForOpenGL(*window, false);
      ImGui_ImplOpenGL3_Init();
      ImGui::StyleColorsDark();
      ImGui::GetIO().MouseDrawCursor = false;
      ImGui::GetIO().IniFilename     = nullptr;
    }

    void UiManager::load_all()
    {
      std::filesystem::directory_iterator iter(UI_DIRECTORY);

      for (auto dir : iter) {
        LOG(INFO) << "loading ui " << dir;
        auto res = File::load(dir);

        if (!res) {
          LOG(WARNING) << "unable to load Ui Xml " << dir;
          continue;
        }

        auto file = res.ok_val();

        this->parse(std::move(file.data));
      }

      this->elements.push_back(std::make_unique<DebugUi>());
    }

    auto UiManager::parse(std::string&& xml) -> bool
    {
      tinyxml2::XMLDocument document;
      auto res = document.Parse(xml.c_str(), xml.size());
      if (res != tinyxml2::XML_SUCCESS) {
        LOG(WARNING) << "unable to parse Ui Xml: " << res;
      }

      auto first               = document.FirstChild();
      std::string element_type = first->Value();

      DLOG(INFO) << "processing element " << element_type;

      if (element_type == UI_EL_WINDOW) {
        auto window = std::make_unique<WindowUi>();
        if (window->parse(first)) {
          this->elements.push_back(std::move(window));
          return true;
        } else {
          return false;
        }
      } else {
        return false;
      }
    }

    void UiManager::render()
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      for (auto& element : this->elements) { element->render(); }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void UiManager::shutdown()
    {
      DLOG(INFO) << "shutting down ui";
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext(this->context);
    }

    WindowUi::WindowUi(std::string t)
     : title(t)
    {}

    void WindowUi::render()
    {
      ImGuiWindowFlags flags = 0;
      flags |= ImGuiWindowFlags_NoResize;

      ImVec2 size{ static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) };
      ImGui::SetNextWindowSize(size);

      if (!this->initial_render) {
        ImVec2 pos{ static_cast<float>(this->pos.x), static_cast<float>(this->pos.y) };
        ImGui::SetNextWindowPos(pos);
        this->initial_render = true;
      }

      ImGui::Begin(this->title.c_str(), nullptr, flags);

      for (const auto& element : elements) { element->render(); }

      ImGui::End();
    }

    auto WindowUi::parse(tinyxml2::XMLNode* self) -> bool
    {
      auto element = self->ToElement();
      if (element == nullptr) {
        LOG(WARNING) << "unable to convert window to element type";
        return false;
      }

      auto title_attr = element->FindAttribute("title");

      if (title_attr == nullptr) {
        LOG(WARNING) << "unable to get title value of window";
        return false;
      }

      this->title = title_attr->Value();
      DLOG(INFO) << "creating window with title " << this->title;

      auto& window = AppWindow::instance();
      auto size    = window.get_size();

      {
        auto width_attr = element->FindAttribute("width");
        if (width_attr == nullptr) {
          this->dim.x = size.x / 2;
        } else {
          float percent{ 0.0 };
          auto res = width_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse width: " << res;
            return false;
          }
          this->dim.x = size.x * percent / 100.0;
        }
      }

      {
        auto height_attr = element->FindAttribute("height");
        if (height_attr == nullptr) {
          this->dim.y = size.y / 2;
        } else {
          float percent{ 0.0 };
          auto res = height_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse height: " << res;
            return false;
          }
          this->dim.y = size.y * percent / 100.0;
        }
      }

      {
        auto x_attr = element->FindAttribute("x");
        if (x_attr == nullptr) {
          this->pos.x = 0;
        } else {
          float percent{ 0.0 };
          auto res = x_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse x pos: " << res;
            return false;
          }
          this->pos.x = size.x * percent / 100.0;
        }
      }

      {
        auto y_attr = element->FindAttribute("x");
        if (y_attr == nullptr) {
          this->pos.y = 0;
        } else {
          float percent{ 0.0 };
          auto res = y_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse x pos: " << res;
            return false;
          }
          this->pos.y = size.y * percent / 100.0;
        }
      }

      return true;
    }

    void DebugUi::render()
    {
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

    void ShaderUi::render()
    {
      auto& shaders = Resources::Shaders::instance();

      if (!this->initial_render) {
        auto& window = AppWindow::instance();
        auto size    = window.get_size();
        ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
        ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
        this->initial_render = true;
      }

      ImGui::Begin("Shaders");

      if (ImGui::CollapsingHeader("Programs")) {
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

            ImGui::Indent(indent);
            ImGui::Text("%s (vertex)", shader_meta.vertex.c_str());
            ImGui::Indent(-indent);

            ImGui::Indent(indent);
            ImGui::Text("%s (fragment)", shader_meta.fragment.c_str());
            ImGui::Indent(-indent);

            if (!program->is_valid()) {
              ImGui::Text("Link Error: %s", program->error().c_str());
            }
          }

          ImGui::PushID(id);
          if (ImGui::Button("Reload")) {
            shaders.reload_program(program_id);
          }
          ImGui::PopID();
          ImGui::Separator();
        }
      }

      if (ImGui::CollapsingHeader("Shaders")) {
        static bool show_src = false;
        static std::string current_shader_id;
        std::size_t id = 0;
        for (auto shader_iter = shaders.shader_begin(); shader_iter != shaders.shader_end(); shader_iter++, id++) {
          auto shader_id = shader_iter->first;
          auto shader    = shader_iter->second;
          ImGui::Text("Shader File: \"%s\"", shader_id.c_str());
          ImGui::SameLine();

          ImGui::PushID(id);
          if (ImGui::Button("Show")) {
            show_src = !show_src;
            if (show_src || current_shader_id != shader_id) {
              show_src          = true;
              current_shader_id = shader_id;
            }
          }
          ImGui::PopID();

          const char* valid_text = shader->is_valid() ? "Ok" : "Error";
          ImGui::Text("Status: %s", valid_text);
          if (!shader->is_valid() && !shader->error().empty()) {
            ImGui::Text("Error: %s", shader->error().c_str());
          }
          ImGui::Separator();
        }

        if (show_src) {
          auto shader_iter = shaders.find_shader(current_shader_id);
          ImGui::Text("Source of %s", current_shader_id.c_str());
          if (ImGui::BeginChild("Source", { 0, 0 }, true)) {
            ImGui::Text("%s", shader_iter->second->get_source().c_str());
          }
          ImGui::EndChild();
        }
      }

      ImGui::End();
    }

    void GlErrorsUi::render()
    {
      if (!this->initial_render) {
        auto& window = AppWindow::instance();
        auto size    = window.get_size();
        ImGui::SetNextWindowSize(ImVec2{ static_cast<float>(size.x) * 0.8f, static_cast<float>(size.y) * 0.8f });
        ImGui::SetNextWindowPos(ImVec2{ static_cast<float>(size.x) * 0.1f, static_cast<float>(size.y) * 0.1f });
        this->initial_render = true;
      }

      auto& errors = GL::ErrorMap::instance();

      ImGui::Begin("OpenGL Errors");

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
            for (const auto& line : lines) {
              ImGui::Indent(indent);
              ImGui::Text("%d: %lu", line.first, line.second);
              ImGui::Indent(-indent);
            }
          }
        }
        ImGui::EndChild();
      }

      ImGui::End();
    }
  }  // namespace Ui
}  // namespace ExpGame
