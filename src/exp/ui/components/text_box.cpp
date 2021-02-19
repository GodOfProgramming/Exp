#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      TextBox::TextBox(std::optional<sol::state>& s)
       : script(s)
      {
        this->enable(true);
      }

      auto TextBox::from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>
      {
        auto self_el = self->ToElement();
        if (self_el == nullptr) {
          LOG(WARNING) << "unable to convert text box to element type";
          return nullptr;
        }

        auto text_box = std::make_shared<TextBox>(script);

        auto el_text = self_el->GetText();
        if (el_text != nullptr) {
          text_box->text = el_text;
        }

        std::string if_func;
        if (UiComponent::has_attr_if(self_el, if_func)) {
          text_box->if_fn = if_func;
        }

        std::string text_func;
        if (UiComponent::has_attr_fn(self_el, text_func)) {
          text_box->text_fn = text_func;
        }

        std::string btn_func;
        if (UiComponent::has_attr_button(self_el, btn_func)) {
          text_box->btn_fn = btn_func;
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
        if (!this->eval_if(this->script)) {
          return;
        }

        if (this->script.has_value() && this->text_fn.has_value()) {
          auto& lua       = this->script.value();
          auto& text_func = this->text_fn.value();
          auto fn         = lua[text_func];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }

        ImGui::Text("%s", this->display_text().c_str());

        if (this->script.has_value() && this->btn_fn.has_value()) {
          ImGui::SameLine();
          if (ImGui::SmallButton(this->btn_text.c_str())) {
            auto& lua      = this->script.value();
            auto& btn_func = this->btn_fn.value();
            auto fn        = lua[btn_func];
            if (fn.get_type() == sol::type::function) {
              fn.call(this);
            }
          }
        }
      }

      auto TextBox::display_text() noexcept -> std::string
      {
        return this->text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
