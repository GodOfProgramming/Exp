#include "repeat_component.hpp"

#include "button.hpp"
#include "exp/constants.hpp"
#include "exp/resources/id.hpp"
#include "frame.hpp"
#include "sameline.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      RepeatComponent::RepeatComponent(std::optional<sol::state_view> script)
       : UiComponent(script)
      {}

      RepeatComponent::~RepeatComponent()
      {
        UiComponent::release();
      }

      auto RepeatComponent::from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [script](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          if (!script.has_value()) {
            LOG(WARNING) << "repeat element window missing script";
            return nullptr;
          }

          std::shared_ptr<RepeatComponent> repeat(new RepeatComponent(script));

          if (!UiComponent::from_node(self, repeat)) {
            return nullptr;
          }

          if (!UiComponent::has_attr_while(self, repeat->while_fn)) {
            LOG(WARNING) << "repeat component missing while attrib";
            return nullptr;
          }

          std::vector<std::shared_ptr<UiComponent>> potential_elements;

          for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
            std::string type = child->Value();

            if (type == El::TEXT_BOX) {
              auto el = TextBox::from_node(child, repeat->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::REPEAT) {
              auto el = RepeatComponent::from_node(child, repeat->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::SAMELINE) {
              auto el = Sameline::from_node(child, repeat->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::BUTTON) {
              auto el = Button::from_node(child, repeat->script);
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

          for (auto el : potential_elements) {
            if (!repeat->add_element(el)) {
              LOG(WARNING) << "could not add element with id " << el->id << ", duplicate id in document";
              return nullptr;
            }
          }

          return repeat;
        });
      }

      void RepeatComponent::render()
      {
        if (this->script.has_value() && !this->eval_if(this->script.value())) {
          return;
        }

        if (this->script.has_value()) {
          auto& lua = this->script.value();
          auto fn   = lua[this->while_fn];

          if (fn.get_type() == sol::type::function) {
            auto& producer = R::ID<int>::producer_t::instance();
            std::vector<R::ID<int>> ids;

            std::size_t index = 0;
            while (fn.call(index++)) {
              auto id = producer.produce();
              ids.push_back(id);
              ImGui::PushID(id.value());
              UiComponent::render_children();
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