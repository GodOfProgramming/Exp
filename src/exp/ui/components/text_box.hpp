#pragma once

#include "exp/resources/id.hpp"
#include "exp/ui/components/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class TextBox: public UiComponent
      {
       public:
        TextBox(std::optional<sol::state_view> script);
        ~TextBox() final;

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state_view& state);

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::string text;
        std::optional<std::string> text_fn;

        std::string btn_text;
        std::optional<std::string> btn_fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp