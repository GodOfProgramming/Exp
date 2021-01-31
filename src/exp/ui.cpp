#include "ui.hpp"

#include "constants.hpp"
#include "gl.hpp"
#include "io.hpp"
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
        LOG(INFO) << "Loading ui " << dir;
        auto res = File::load(dir);

        if (!res) {
          LOG(WARNING) << "Unable to load Ui Xml " << dir;
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
        LOG(WARNING) << "Unable to parse Ui Xml: " << res;
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
      DLOG(INFO) << "Shutting down ui";
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext(this->context);
    }

    void DebugUi::render()
    {
      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("debug")) {
          if (ImGui::MenuItem("demo window")) {
            this->show_demo_window = !this->show_demo_window;
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
    }

    WindowUi::WindowUi(std::string t)
     : title(t)
    {}

    void WindowUi::render()
    {
      ImGuiWindowFlags flags = 0;
      flags |= ImGuiWindowFlags_NoResize;

      ImVec2 size{this->dim.x, this->dim.y};
      ImGui::SetNextWindowSize(size);

      if (!this->initial_render) {
        ImVec2 pos{this->pos.x, this->pos.y};
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
        LOG(WARNING) << "Unable to convert window to element type";
        return false;
      }

      auto title_attr = element->FindAttribute("title");

      if (title_attr == nullptr) {
        LOG(WARNING) << "Unable to get title value of window";
        return false;
      }

      this->title = title_attr->Value();
      DLOG(INFO) << "Creating window with title " << this->title;

      auto& window = AppWindow::instance();
      auto size    = window.get_size();

      {
        auto width_attr = element->FindAttribute("width");
        if (width_attr == nullptr) {
          this->dim.x = size.x / 2;
        } else {
          float percent{0.0};
          auto res = width_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "Unable to parse width: " << res;
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
          float percent{0.0};
          auto res = height_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "Unable to parse height: " << res;
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
          float percent{0.0};
          auto res = x_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "Unable to parse x pos: " << res;
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
          float percent{0.0};
          auto res = y_attr->QueryFloatValue(&percent);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "Unable to parse x pos: " << res;
            return false;
          }
          this->pos.y = size.y * percent / 100.0;
        }
      }

      return true;
    }
  }  // namespace Ui
}  // namespace ExpGame
