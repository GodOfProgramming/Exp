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
       , text(t)
      {
        this->enable(true);
      }

      auto TextBox::from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>
      {
        std::string fn;
        auto self_el = self->ToElement();
        auto text    = self_el->GetText();
        auto fn_attr = self_el->FindAttribute("fn");
        if (fn_attr != nullptr) {
          fn = fn_attr->Value();
        }

        auto text_box = std::make_shared<TextBox>(script, fn, text == nullptr ? "" : text);

        auto button_attr = self_el->FindAttribute("button");
        if (button_attr != nullptr) {
          text_box->button_fn = button_attr->Value();
        }

        return text_box;
      }

      void TextBox::add_usertype(sol::state& state)
      {
        state.new_usertype<TextBox>(
         "TextBox", "text", &TextBox::text, "btn_text", &TextBox::btn_text, "is_enabled", &TextBox::is_enabled, "enable", &TextBox::enable);
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

        ImGui::Text("%s", this->display_text().c_str());
        if (this->button_fn.has_value()) {
          ImGui::SameLine();
          ImGui::PushID(this->btn_id.value());
          if (ImGui::SmallButton(this->btn_text.c_str())) {
            if (this->script.has_value() && this->button_fn.has_value()) {
              auto& lua   = this->script.value();
              auto& btnfn = this->button_fn.value();
              auto fn     = lua[btnfn];
              if (fn.get_type() == sol::type::function) {
                fn.call(this);
              }
            }
          }
          ImGui::PopID();
        }
      }

      auto TextBox::display_text() noexcept -> std::string
      {
        return this->text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
