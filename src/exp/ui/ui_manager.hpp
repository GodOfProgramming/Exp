#pragma once

#include "components/ui_component.hpp"
#include "exp/input/ihandler.hpp"

namespace Exp
{
  namespace Ui
  {
    class UiManager: public Input::IHandler
    {
      UiManager();

     public:
      UiManager(const UiManager&) = delete;
      UiManager(UiManager&&)      = delete;
      auto operator=(const UiManager&) -> UiManager& = delete;
      auto operator=(UiManager&&) -> UiManager& = delete;

      ~UiManager() = default;

      static auto instance() -> UiManager&;

      void load_all();

      void render();

      void shutdown();

      auto parse(const std::string_view& xml) -> bool;

      auto handle(Input::KeyEvent) -> IHandler* override;
      auto handle(Input::MouseButtonEvent) -> IHandler* override;
      auto handle(Input::MouseMoveEvent) -> IHandler* override;

     private:
      ImGuiContext* context;
      std::vector<std::shared_ptr<UiComponent>> elements;
      std::vector<std::shared_ptr<UiComponent>> debug_elements;
      bool show_demo_window;
      bool show_menu_bar;
    };
  }  // namespace Ui
}  // namespace Exp