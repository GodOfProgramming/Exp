#include "repeat_component.hpp"

#include "exp/constants.hpp"
#include "exp/resources/id.hpp"
#include "exp/ui/components/text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      RepeatComponent::RepeatComponent(std::optional<sol::state>& s)
       : script(s)
      {}

      auto RepeatComponent::from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>
      {
        if (!script.has_value()) {
          LOG(WARNING) << "repeat element window missing script";
          return nullptr;
        }

        auto self_el = self->ToElement();
        if (self_el == nullptr) {
          LOG(WARNING) << "unable to convert repeat component to element type";
          return nullptr;
        }

        auto repeat = std::make_shared<RepeatComponent>(script);

        std::string if_func;
        if (UiComponent::has_attr_if(self_el, if_func)) {
          repeat->if_fn = if_func;
        }

        if (!UiComponent::has_attr_while(self_el, repeat->while_fn)) {
          LOG(WARNING) << "repeat component missing while attrib";
          return nullptr;
        }

        decltype(repeat->elements) potential_elements;

        for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
          std::string type = child->Value();

          if (type == UI_EL_TEXT_BOX) {
            auto el = TextBox::from_node(child, repeat->script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else if (type == UI_EL_REPEAT) {
            auto el = RepeatComponent::from_node(child, repeat->script);
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
        if (!this->eval_if(this->script)) {
          return;
        }

        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua[this->while_fn];

          if (fn.get_type() == sol::type::function) {
            auto& producer = R::ID<int>::producer_t::instance();
            std::vector<R::ID<int>> ids;

            while (fn.call()) {
              auto id = producer.produce();
              ids.push_back(id);
              ImGui::PushID(id.value());
              for (const auto& element : elements) { element->render(); }
              ImGui::PopID();
            }

            for (auto id : ids) { id.release(); }
          }
        }
      }

      auto RepeatComponent::display_text() noexcept -> std::string
      {
        return std::string();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp