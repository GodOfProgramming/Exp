#include "ui_manager.hpp"

#include "components/gl_errors_ui.hpp"
#include "components/shader_ui.hpp"
#include "components/window_ui.hpp"
#include "exp/constants.hpp"
#include "exp/io.hpp"
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
     : show_demo_window(false)
     , show_menu_bar(false)
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
      LOG(INFO) << "loading ui";
      using IO::File;
      IO::iterate_dir_with_namespace(Assets::Dir::UI, "exp", [&](std::filesystem::path path, std::string) {
        LOG(INFO) << "loading ui file " << path;
        File::load(path, [&](const std::string_view& src) { this->parse(src); });
      });
      this->debug_elements.push_back(std::make_shared<Components::ShaderUi>());
      this->debug_elements.push_back(std::make_shared<Components::GlErrorsUi>());
    }

    auto UiManager::parse(const std::string_view& xml) -> bool
    {
      using Components::WindowUi;

      tinyxml2::XMLDocument document;
      auto res = document.Parse(xml.data(), xml.length());
      if (res != tinyxml2::XML_SUCCESS) {
        LOG(WARNING) << "unable to parse Ui Xml: " << res;
        return false;
      }

      auto first               = document.FirstChild();
      std::string element_type = first->Value();

      if (element_type == El::WINDOW) {
        auto window = WindowUi::from_node(first);
        if (window) {
          this->elements.push_back(window);
        } else {
          LOG(WARNING) << "was not able to create window";
          return false;
        }
      } else {
        LOG(WARNING) << "first node of xml is not a recognized or valid type";
        return false;
      }
      return true;
    }

    void UiManager::render()
    {
      using Render::AppWindow;

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      if (this->show_menu_bar) {
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("Windows")) {
            for (auto& el : this->elements) {
              if (ImGui::MenuItem(el->display_text().c_str())) {
                el->enable(!el->is_enabled());
              }
            }
            ImGui::EndMenu();
          }

          if (ImGui::BeginMenu("Debug")) {
            for (auto& el : this->debug_elements) {
              if (ImGui::MenuItem(el->display_text().c_str())) {
                el->enable(!el->is_enabled());
              }
            }

            if (ImGui::MenuItem("Ui Demo Window")) {
              this->show_demo_window = !this->show_demo_window;
            }

            ImGui::EndMenu();
          }

          if (ImGui::BeginMenu("Exit")) {
            auto& window = AppWindow::instance();
            window.close();
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
      }

      for (auto& el : this->elements) {
        if (el->is_enabled()) {
          el->render();
        }
      }

      for (auto& el : this->debug_elements) {
        if (el->is_enabled()) {
          el->render();
        }
      }

      if (this->show_demo_window) {
        ImGui::ShowDemoWindow();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void UiManager::release()
    {
      this->elements.clear();
      this->debug_elements.clear();
      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext(this->context);
    }

    auto UiManager::handle(Input::KeyEvent e) -> IHandler*
    {
      using Input::Key;
      switch (e.key) {
        case Key::F4: {
          if (e.action == Input::Action::Press) {
            this->show_menu_bar = !this->show_menu_bar;
          }
        } break;
        default: {
          return this->get_next();
        }
      }

      return nullptr;
    }

    auto UiManager::handle(Input::MouseButtonEvent) -> IHandler*
    {
      return this->get_next();
    }

    auto UiManager::handle(Input::MouseMoveEvent) -> IHandler*
    {
      return this->get_next();
    }

  }  // namespace Ui
}  // namespace Exp