#include "frame.hpp"

#include "exp/constants.hpp"
#include "exp/game/info.hpp"
#include "exp/resources/producer.hpp"
#include "exp/resources/scripts.hpp"
#include "repeat_component.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      Frame::Frame(const Container& c, std::optional<sol::state>& ps)
       : container(c)
       , parent_script(ps)
      {}

      auto Frame::from_node(tinyxml2::XMLNode* self, const Container& container, std::optional<sol::state>& parent_script) -> std::shared_ptr<UiComponent>
      {
        using Game::Info;
        using R::Scripts;

        auto self_el = self->ToElement();

        auto frame = std::make_shared<Frame>(container, parent_script);

        {
          auto script_attr = self_el->FindAttribute("script");
          if (script_attr != nullptr) {
            std::string script_key = script_attr->Value();
            auto& scripts          = Scripts::instance();

            scripts.make_script(script_key, frame->script, [](sol::state& state) {
              Frame::add_usertype(state);
              TextBox::add_usertype(state);
              Info::add_usertype(state);
              return true;
            });

            if (!frame->script.has_value()) {
              LOG(WARNING) << "unable to load script " << script_key;
              return nullptr;
            }
          }
        }

        decltype(frame->elements) potential_elements;

        for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
          std::string type = child->Value();

          auto& child_script = frame->script.has_value() ? frame->script : parent_script;

          if (type == UI_EL_TEXT_BOX) {
            auto el = TextBox::from_node(child, child_script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else if (type == UI_EL_REPEAT) {
            auto el = RepeatComponent::from_node(child, child_script);
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

        frame->elements = std::move(potential_elements);

        return frame;
      }

      void Frame::add_usertype(sol::state& state)
      {
        state.new_usertype<Frame>("Frame");
      }

      auto Frame::width() const noexcept -> int
      {
        return this->dim.x;
      }

      auto Frame::height() const noexcept -> int
      {
        return this->dim.y;
      }

      void Frame::render()
      {
        float w = this->container.width();
        float h = this->container.height();

        if (ImGui::BeginChildFrame(id.value(), { w, h / 2.0f })) {
          auto sz   = ImGui::GetItemRectSize();
          this->dim = { sz.x, sz.y };

          for (const auto& element : elements) { element->render(); }
        }
        ImGui::EndChildFrame();
      }

      auto Frame::text() noexcept -> std::string
      {
        return std::string();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp