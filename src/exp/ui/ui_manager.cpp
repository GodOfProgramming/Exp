#include "ui_manager.hpp"

#include "components/debug_ui.hpp"
#include "components/shader_ui.hpp"
#include "components/window_ui.hpp"
#include "exp/constants.hpp"
#include "exp/io/file.hpp"
#include "exp/render/app_window.hpp"

namespace Exp
{
  namespace Ui
  {
    auto UiManager::instance() -> UiManager&
    {
      static UiManager manager;
      return manager;
    }

    UiManager::UiManager()
    {
      using Render::AppWindow;

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
      using Components::DebugUi;
      using IO::File;

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
      using Components::WindowUi;

      tinyxml2::XMLDocument document;
      auto res = document.Parse(xml.c_str(), xml.size());
      if (res != tinyxml2::XML_SUCCESS) {
        LOG(WARNING) << "unable to parse Ui Xml: " << res;
      }

      auto first               = document.FirstChild();
      std::string element_type = first->Value();

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
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext(this->context);
    }
  }  // namespace Ui
}  // namespace Exp