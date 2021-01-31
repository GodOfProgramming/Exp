#pragma once

#include "util.hpp"

namespace ExpGame
{
  namespace Ui
  {
    class Ui
    {
     public:
      virtual ~Ui() = default;

      virtual void render() = 0;
    };

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
      std::vector<std::unique_ptr<Ui>> elements;
    };

    class DebugUi: public Ui
    {
     public:
      virtual ~DebugUi() override = default;

      void render() final;

     private:
      bool show_demo_window = false;
    };

    class WindowUi: public Ui
    {
     public:
      WindowUi() = default;
      WindowUi(std::string title);

      void render() final;

      auto parse(tinyxml2::XMLNode* self) -> bool;

     private:
      std::string title;
      glm::ivec2 pos;
      glm::ivec2 dim;
      bool initial_render = false;
      std::vector<std::unique_ptr<Ui>> elements;
    };
  }  // namespace Ui
}  // namespace ExpGame
