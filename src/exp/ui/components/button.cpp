#include "button.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      Button::Button(std::optional<sol::state_view> s)
       : UiComponent(s)
      {
        this->enable(true);
      }

      Button::~Button()
      {
        UiComponent::release();
      }

      auto Button::from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [script](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          std::shared_ptr<Button> button(new Button(script));

          if (!UiComponent::from_node(self, button)) {
            return nullptr;
          }

          if (!UiComponent::has_attr_click(self, button->fn)) {
            LOG(WARNING) << "unable to create button, missing required click attribute";
            return nullptr;
          }

          auto el_text = self->GetText();
          if (el_text != nullptr) {
            button->text = el_text;
          }

          return button;
        });
      }

      void Button::render()
      {
        if (this->script.has_value() && !this->fn.empty()) {
          if (ImGui::Button(this->text.c_str())) {
            auto& lua = this->script.value();
            auto fn   = lua[this->fn];
            if (fn.get_type() == sol::type::function) {
              fn.call(this);
            }
          }
        }
      }

      auto Button::display_text() noexcept -> std::string
      {
        return this->text;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp