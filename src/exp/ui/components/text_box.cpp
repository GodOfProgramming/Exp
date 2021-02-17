#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      TextBox::TextBox(std::optional<sol::state>& s, std::string f, std::string t)
       : script(s)
       , function(f)
       , display_text(t)
      {
        this->enable(true);
      }

      auto TextBox::from_node(tinyxml2::XMLNode* child, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>
      {
        std::string fn;
        auto child_el = child->ToElement();
        auto text     = child_el->GetText();
        auto fn_attr  = child_el->FindAttribute("fn");
        if (fn_attr != nullptr) {
          fn = fn_attr->Value();
        }

        return std::make_shared<TextBox>(script, fn, text == nullptr ? "" : text);
      }

      void TextBox::add_usertype(sol::state& state)
      {
        state.new_usertype<TextBox>(
         "TextBox", "display_text", &TextBox::display_text, "text", &TextBox::text, "is_enabled", &TextBox::is_enabled, "enable", &TextBox::enable);
      }

      void TextBox::render()
      {
        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua[this->function];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }

        ImGui::Text("%s", this->display_text.c_str());
      }

      auto TextBox::text() noexcept -> std::string
      {
        return this->display_text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
