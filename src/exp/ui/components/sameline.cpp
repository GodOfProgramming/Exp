#include "sameline.hpp"

#include "button.hpp"
#include "exp/constants.hpp"
#include "repeat_component.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      Sameline::Sameline(std::optional<sol::state_view> s)
       : UiComponent(s)
      {
        this->enable(true);
      }

      Sameline::~Sameline()
      {
        UiComponent::release();
      }

      auto Sameline::from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [script](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          std::vector<std::shared_ptr<UiComponent>> potential_elements;

          std::shared_ptr<Sameline> sameline(new Sameline(script));

          if (!UiComponent::from_node(self, sameline)) {
            return nullptr;
          }

          for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
            std::string type = child->Value();

            if (type == El::TEXT_BOX) {
              auto el = TextBox::from_node(child, sameline->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::REPEAT) {
              auto el = RepeatComponent::from_node(child, sameline->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::SAMELINE) {
              auto el = Sameline::from_node(child, sameline->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::BUTTON) {
              auto el = Button::from_node(child, sameline->script);
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
            if (!sameline->add_element(el)) {
              LOG(WARNING) << "could not add element with id " << el->id << ", duplicate id in document";
              return nullptr;
            }
          }

          return sameline;
        });
      }

      void Sameline::render()
      {
        for (auto child = this->begin(); child != this->end(); child++) {
          (*child)->render();

          if (child + 1 != this->end()) {
            ImGui::SameLine();
          }
        }
      }

      auto Sameline::display_text() noexcept -> std::string
      {
        return std::string();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp