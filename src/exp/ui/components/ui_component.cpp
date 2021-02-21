#include "ui_component.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace Ui
  {
    UiComponent::UiComponent(std::optional<sol::state_view> s)
     : script(s)
    {
      this->secret_id = R::ID<std::size_t>::producer_t::instance().produce();
    }

    UiComponent::~UiComponent()
    {
      this->secret_id.release();
    }

    void UiComponent::release()
    {
      this->script = std::nullopt;
      this->elements.clear();
      this->element_map.clear();
    }

    auto UiComponent::unwrap_node(tinyxml2::XMLNode* node, std::function<std::shared_ptr<UiComponent>(tinyxml2::XMLElement*)> callback)
     -> std::shared_ptr<UiComponent>
    {
      auto el = node->ToElement();
      if (el == nullptr) {
        LOG(WARNING) << "unable to convert xml node to element type";
        return nullptr;
      }

      return callback(el);
    }

    auto UiComponent::from_node(tinyxml2::XMLElement* self, std::shared_ptr<UiComponent> cmp) -> bool
    {
      UiComponent::has_attr_id(self, cmp->id);

      std::string if_func;
      if (UiComponent::has_attr_if(self, if_func)) {
        cmp->if_fn = if_func;
      }

      return true;
    }

    void UiComponent::enable(bool value) noexcept
    {
      this->enabled = value;
    }

    auto UiComponent::is_enabled() const noexcept -> bool
    {
      return this->enabled;
    }

    auto UiComponent::add_element(std::shared_ptr<UiComponent> el) -> bool
    {
      this->elements.push_back(el);
      if (!el->id.empty()) {
        if (this->element_map.find(id) == this->element_map.end()) {
          this->element_map[el->id] = el;
        } else {
          return false;
        }
      }
      return true;
    }

    auto UiComponent::has_attr_onparse(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto onparsed_attr = self->FindAttribute(Attr::ON_PARSED);

      if (onparsed_attr == nullptr) {
        return false;
      }

      fn = onparsed_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_script(tinyxml2::XMLElement* self, std::string& script) -> bool
    {
      auto script_attr = self->FindAttribute(Attr::SCRIPT);

      if (script_attr == nullptr) {
        return false;
      }

      script = script_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_id(tinyxml2::XMLElement* self, std::string& id) -> bool
    {
      auto id_attr = self->FindAttribute(Attr::ID);

      if (id_attr == nullptr) {
        return false;
      }

      id = id_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_if(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto if_attr = self->FindAttribute(Attr::IF);

      if (if_attr == nullptr) {
        return false;
      }

      fn = if_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_click(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto click_attr = self->FindAttribute(Attr::CLICK);

      if (click_attr == nullptr) {
        return false;
      }

      fn = click_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_fn(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto fn_attr = self->FindAttribute(Attr::FN);

      if (fn_attr == nullptr) {
        return false;
      }

      fn = fn_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_button(tinyxml2::XMLElement* self, std::string& fn) -> bool
    {
      auto button_fn = self->FindAttribute(Attr::BUTTON);

      if (button_fn == nullptr) {
        return false;
      }

      fn = button_fn->Value();
      return true;
    }

    auto UiComponent::has_attr_title(tinyxml2::XMLElement* self, std::string& title) -> bool
    {
      auto title_attr = self->FindAttribute(Attr::TITLE);

      if (title_attr == nullptr) {
        return false;
      }

      title = title_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_collapsed(tinyxml2::XMLElement* self, bool& is_collapsed) -> bool
    {
      auto collapsed_attr = self->FindAttribute(Attr::COLLAPSED);

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
      auto fixed_attr = self->FindAttribute(Attr::FIXED);

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
      auto while_attr = self->FindAttribute(Attr::WHILE);

      if (while_attr == nullptr) {
        return false;
      }

      fn = while_attr->Value();
      return true;
    }

    auto UiComponent::has_attr_width(tinyxml2::XMLElement* self, float& width) -> bool
    {
      auto width_attr = self->FindAttribute(Attr::WIDTH);
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
      auto height_attr = self->FindAttribute(Attr::HEIGHT);
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
      auto x_attr = self->FindAttribute(Attr::X);
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
      auto y_attr = self->FindAttribute(Attr::Y);
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

    auto UiComponent::eval_if(sol::state_view lua) const -> bool
    {
      if (this->if_fn.has_value()) {
        auto& if_fn = this->if_fn.value();
        auto fn     = lua[if_fn];
        if (fn.get_type() == sol::type::function) {
          return fn.call();
        }
      }
      return true;
    }

    auto UiComponent::begin() -> ElementList::iterator
    {
      return this->elements.begin();
    }

    auto UiComponent::end() -> ElementList::iterator
    {
      return this->elements.end();
    }

    void UiComponent::render_children()
    {
      for (const auto& element : this->elements) { element->render(); }
    }
  }  // namespace Ui
}  // namespace Exp
