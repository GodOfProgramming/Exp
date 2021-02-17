#include "repeat_component.hpp"

#include "exp/constants.hpp"
#include "exp/ui/components/text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      RepeatComponent::RepeatComponent(std::optional<sol::state>& s, std::string f)
       : opt_script(s)
       , script(opt_script.value())
       , function(f)
      {}

      auto RepeatComponent::from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>
      {
        if (!script.has_value()) {
          LOG(WARNING) << "repeat element window missing script";
          return nullptr;
        }

        std::string func_name;
        auto self_el = self->ToElement();
        auto fn_attr = self_el->FindAttribute("while");
        if (fn_attr != nullptr) {
          func_name = fn_attr->Value();
        }

        auto& lua = script.value();
        auto fn   = lua[func_name];
        if (fn.get_type() != sol::type::function) {
          LOG(WARNING) << "associated function for repeat element is not a function";
          return nullptr;
        }

        auto repeat = std::make_shared<RepeatComponent>(script, func_name);

        decltype(repeat->elements) potential_elements;

        for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
          std::string type = child->Value();

          if (type == UI_EL_TEXT_BOX) {
            auto el = TextBox::from_node(child, repeat->opt_script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else if (type == UI_EL_REPEAT) {
            auto el = RepeatComponent::from_node(child, repeat->opt_script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else {
            LOG(WARNING) << "invalid type detected " << type;
            return nullptr;
          }
        }

        repeat->elements = std::move(potential_elements);

        return repeat;
      }

      void RepeatComponent::render()
      {
        auto fn = this->script[this->function];
        if (fn.get_type() == sol::type::function) {
          while (fn.call()) {
            for (const auto& element : elements) { element->render(); }
          }
        }
      }

      auto RepeatComponent::text() noexcept -> std::string
      {
        return std::string();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp