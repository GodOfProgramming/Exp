#include "ui_component.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Ui
  {
    void UiComponent::enable(bool value) noexcept
    {
      this->enabled = value;
    }

    auto UiComponent::is_enabled() const noexcept -> bool
    {
      return this->enabled;
    }

    auto UiComponent::has_attr_script(tinyxml2::XMLElement* self, std::string& script) -> bool
    {
      auto script_attr = self->FindAttribute(UI_ATTR_SCRIPT);

      if (script_attr == nullptr) {
        return false;
      }

      script = script_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_if(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto if_attr = self->FindAttribute(UI_ATTR_IF);

      if (if_attr == nullptr) {
        return false;
      }

      fn = if_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_fn(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto fn_attr = self->FindAttribute(UI_ATTR_FN);

      if (fn_attr == nullptr) {
        return false;
      }

      fn = fn_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_button(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto button_fn = self->FindAttribute(UI_ATTR_BUTTON);

      if (button_fn == nullptr) {
        return false;
      }

      fn = button_fn->Value();
      return true;
    }

    auto UiComponent::has_attr_title(tinyxml2::XMLElement* self, std::string& title) -> bool
    {
      auto title_attr = self->FindAttribute(UI_ATTR_TITLE);

      if (title_attr == nullptr) {
        return false;
      }

      title = title_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_collapsed(tinyxml2::XMLElement* self, bool& is_collapsed) -> bool
    {
      auto collapsed_attr = self->FindAttribute(UI_ATTR_COLLAPSED);

      if (collapsed_attr != nullptr) {
        bool collapsed = false;
        auto res       = collapsed_attr->QueryBoolValue(&collapsed);
        if (res == tinyxml2::XML_SUCCESS) {
          is_collapsed = collapsed;
          return true;
        }
      }
      return false;
    }

    auto UiComponent::has_attr_fixed(tinyxml2::XMLElement* self, bool& is_fixed) -> bool
    {
      auto fixed_attr = self->FindAttribute(UI_ATTR_FIXED);

      if (fixed_attr != nullptr) {
        bool fixed = false;
        auto res   = fixed_attr->QueryBoolValue(&fixed);
        if (res == tinyxml2::XML_SUCCESS) {
          is_fixed = fixed;
          return true;
        }
      }

      return false;
    }

    auto UiComponent::has_attr_while(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto while_attr = self->FindAttribute(UI_ATTR_WHILE);

      if (while_attr == nullptr) {
        return false;
      }

      fn = while_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_width(tinyxml2::XMLElement* self, float& width) -> bool
    {
      auto width_attr = self->FindAttribute(UI_ATTR_WIDTH);
      if (width_attr != nullptr) {
        float percent{ 0.0f };
        auto res = width_attr->QueryFloatValue(&percent);
        if (res == tinyxml2::XML_SUCCESS) {
          width = percent;
          return true;
        }
      }
      return false;
    }

    auto UiComponent::has_attr_height(tinyxml2::XMLElement* self, float& height) -> bool
    {
      auto height_attr = self->FindAttribute(UI_ATTR_HEIGHT);
      if (height_attr != nullptr) {
        float percent{ 0.0f };
        auto res = height_attr->QueryFloatValue(&percent);
        if (res == tinyxml2::XML_SUCCESS) {
          height = percent;
          return true;
        }
      }
      return false;
    }

    auto UiComponent::has_attr_x(tinyxml2::XMLElement* self, float& x) -> bool
    {
      auto x_attr = self->FindAttribute(UI_ATTR_X);
      if (x_attr != nullptr) {
        float percent{ 0.0f };
        auto res = x_attr->QueryFloatValue(&percent);
        if (res == tinyxml2::XML_SUCCESS) {
          x = percent;
          return true;
        }
      }
      return false;
    }

    auto UiComponent::has_attr_y(tinyxml2::XMLElement* self, float& y) -> bool
    {
      auto y_attr = self->FindAttribute(UI_ATTR_Y);
      if (y_attr != nullptr) {
        float percent{ 0.0f };
        auto res = y_attr->QueryFloatValue(&percent);
        if (res == tinyxml2::XML_SUCCESS) {
          y = percent;
          return true;
        }
      }
      return false;
    }

    auto UiComponent::eval_if(std::optional<sol::state>& scriptopt) const -> bool
    {
      if (this->if_fn.has_value() && scriptopt.has_value()) {
        auto& script = scriptopt.value();
        auto& if_fn  = this->if_fn.value();
        auto fn      = script[if_fn];
        if (fn.get_type() == sol::type::function) {
          return fn.call();
        }
      }
      return true;
    }
  }  // namespace Ui
}  // namespace Exp
