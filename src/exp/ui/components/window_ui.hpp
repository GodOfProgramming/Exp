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

        static void add_usertype(sol::state& state);

        auto parse(tinyxml2::XMLNode* self) -> bool;

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        std::string title;
        glm::ivec2 dim;
        glm::ivec2 pos;
        bool initial_render;
        bool is_collapsed;
        std::vector<std::unique_ptr<UiComponent>> elements;
        std::optional<sol::state> lua;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp