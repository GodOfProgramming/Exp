#include "ui.hpp"

#include "constants.hpp"
#include "window.hpp"

namespace ExpGame
{
  auto UiManager::instance() -> UiManager&
  {
    static UiManager manager;
    return manager;
  }

  UiManager::UiManager()
  {
    IMGUI_CHECKVERSION();
    this->context = ImGui::CreateContext();

    auto& window = Window::instance();
    ImGui_ImplGlfw_InitForOpenGL(*window, false);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();
    ImGui::GetIO().MouseDrawCursor = false;
    ImGui::GetIO().IniFilename     = nullptr;
  }

  void UiManager::load_all()
  {
    std::filesystem::directory_iterator iter(UI_DIRECTORY);

    for (auto dir : iter) { DLOG(INFO) << "loading ui " << dir; }

    this->elements.push_back(std::make_shared<DebugUi>());
  }

  void UiManager::render()
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto element : this->elements) { element->render(); }

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
          auto& window = Window::instance();
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
}  // namespace ExpGame
