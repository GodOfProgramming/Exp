#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      TextBox::TextBox(std::optional<sol::state_view> s)
       : UiComponent(s)
      {
        this->enable(true);
      }

      TextBox::~TextBox()
      {
        UiComponent::release();
      }

      auto TextBox::from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [script](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          std::shared_ptr<TextBox> text_box(new TextBox(script));

          if (!UiComponent::from_node(self, text_box)) {
            return nullptr;
          }

          auto el_text = self->GetText();
          if (el_text != nullptr) {
            text_box->text = el_text;
          }

          std::string text_func;
          if (UiComponent::has_attr_fn(self, text_func)) {
            text_box->text_fn = text_func;
          }

          return text_box;
        });
      }

      void TextBox::add_usertype(sol::state_view state)
      {
        state.new_usertype<TextBox>("TextBox", "text", &TextBox::text, "is_enabled", &TextBox::is_enabled, "enable", &TextBox::enable);
      }

      void TextBox::render()
      {
        if (this->script.has_value() && !this->eval_if(this->script.value())) {
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
      }

      auto TextBox::display_text() noexcept -> std::string
      {
        return this->text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp
