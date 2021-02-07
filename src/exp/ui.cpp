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

      ImVec2 size{ this->dim.x, this->dim.y };
      ImGui::SetNextWindowSize(size);

      if (!this->initial_render) {
        ImVec2 pos{ this->pos.x, this->pos.y };
        ImGui::SetNextWindowPos(pos);
        this->initial_render = true;
      }

      ImGui::Begin(this->title.c_str(), nullptr, flags);

      for (const auto& element : elements) { element->render(); }

      auto& errors = GL::ErrorMap::instance();

      for (const auto& pair : errors) {}

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
        if (ImGui::BeginMenu("debug")) {
          if (ImGui::MenuItem("demo window")) {
            this->show_demo_window = !this->show_demo_window;
          }

          if (ImGui::MenuItem("shaders")) {
            this->show_shader_debug = !this->show_shader_debug;
          }

          if (ImGui::MenuItem("exit")) {
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
        ShaderUi ui;
        ui.render();
      }
    }

    void ShaderUi::render()
    {
      const auto& shaders = Resources::Shaders::instance();

      if (!this->initial_render) {
        auto& window = AppWindow::instance();
        auto size    = window.get_size();
        ImGui::SetNextWindowSize(ImVec2{ size.x * 0.8, size.y * 0.8 });
        ImGui::SetNextWindowPos(ImVec2{ size.x * 0.1, size.y * 0.1 });
        this->initial_render = true;
      }

      ImGui::Begin("Shaders");

      if (ImGui::CollapsingHeader("Programs")) {
        for (auto cfg_iter = shaders.cache_begin(); cfg_iter != shaders.cache_end(); cfg_iter++) {
          auto& program_id  = cfg_iter->first;
          auto& shader_meta = cfg_iter->second;

          ImGui::Text("ID: \"%s\"", program_id.c_str());

          const auto program_iter = shaders.find_program(cfg_iter->first);

          bool loaded = program_iter != shaders.program_end();

          const char* status_txt = loaded ? "Yes" : "No";
          ImGui::Text("Loaded: %s", status_txt);

          if (loaded) {
            const auto& program = program_iter->second;

            for (const auto& shader_file : shader_meta.shaders) {
              ImGui::Indent(1.0f);
              ImGui::Text("%s", shader_file.c_str());
            }

            if (!program.is_valid()) {
              ImGui::Text("Link Error: %s", program.error().c_str());
            }
          }
          ImGui::Separator();
        }
      }

      if (ImGui::CollapsingHeader("Shaders")) {
        static bool show_src = false;
        static std::string current_id;
        static std::string current_src;
        std::size_t id = 0;
        for (auto shader_iter = shaders.shader_begin(); shader_iter != shaders.shader_end(); shader_iter++, id++) {
          auto& shader_id = shader_iter->first;
          auto& shader    = shader_iter->second;
          ImGui::Text("Shader File: \"%s\"", shader_id.c_str());
          ImGui::SameLine();

          ImGui::PushID(id);
          if (ImGui::Button("Show")) {
            show_src = !show_src;
            if (current_id != shader_id) {
              show_src    = true;
              current_id  = shader_id;
              current_src = shader.get_source();
            }
          }
          ImGui::PopID();

          const char* valid_text = shader.is_valid() ? "Ok" : "Error";
          ImGui::Text("Status: %s", valid_text);
          if (!shader.is_valid()) {
            ImGui::Text("%s", shader.error().c_str());
          }
          ImGui::Separator();
        }

        if (show_src) {
          ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
          ImGui::BeginChild("Source", ImVec2{ 0, 260 }, true);
          ImGui::Text("Source of %s", current_id.c_str());
          ImGui::Text("%s", current_src.c_str());
          ImGui::EndChild();
          ImGui::PopStyleVar();
        }
      }

      ImGui::End();
    }
  }  // namespace Ui
}  // namespace ExpGame
