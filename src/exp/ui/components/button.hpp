#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Button: public UiComponent
      {
       public:
        Button(std::optional<sol::state_view> script);
        ~Button();

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::string text;
        std::string fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp