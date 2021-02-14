#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    class UiManager
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

      auto parse(std::string&& xml) -> bool;

     private:
      ImGuiContext* context;
      std::vector<std::unique_ptr<UiComponent>> elements;
    };
  }  // namespace Ui
}  // namespace Exp