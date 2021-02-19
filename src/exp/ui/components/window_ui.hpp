#pragma once

#include "window_ui.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class WindowUi: public IWindowUi
      {
       public:
        WindowUi();

        static auto from_node(tinyxml2::XMLNode* self) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state& state);

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        std::optional<sol::state> lua;

        std::string title;
        glm::ivec2 dim;
        glm::ivec2 pos;
        bool initial_render;
        bool is_collapsed;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp