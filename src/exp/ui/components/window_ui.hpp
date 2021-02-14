#pragma once

#include "exp/ui/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class WindowUi: public UiComponent
      {
       public:
        WindowUi() = default;
        WindowUi(std::string title);

        void render() final;

        auto parse(tinyxml2::XMLNode* self) -> bool;

       private:
        std::string title;
        glm::ivec2 dim;
        glm::ivec2 pos;
        bool hide;
        bool initial_render;
        bool is_collapsed;
        std::vector<std::unique_ptr<UiComponent>> elements;
        std::optional<sol::state> lua;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp