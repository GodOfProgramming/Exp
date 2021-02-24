#include "frame.hpp"

#include "button.hpp"
#include "exp/constants.hpp"
#include "exp/game/info.hpp"
#include "exp/resources/producer.hpp"
#include "exp/resources/scripts.hpp"
#include "repeat_component.hpp"
#include "sameline.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      Frame::Frame(std::optional<sol::environment> env, const Container& c)
       : Container(env)
       , container(c)
      {
        auto& producer = R::ID<ImGuiID>::producer_t::instance();
        this->imgui_id = producer.produce();
      }

      Frame::~Frame()
      {
        Container::release();
        this->imgui_id.release();
      }

      auto Frame::from_node(tinyxml2::XMLNode* self, std::optional<sol::environment> env, const Container& container) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [env, &container](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          using Game::Info;
          using R::Scripts;

          std::shared_ptr<Frame> frame(new Frame(env, container));
          glm::ivec2 size = { frame->container.width(), frame->container.height() };

          if (!Container::from_node(self, frame, size)) {
            return nullptr;
          }

          std::vector<std::shared_ptr<UiComponent>> potential_elements;

          for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
            std::string type = child->Value();

            if (type == El::TEXT_BOX) {
              auto el = TextBox::from_node(child, frame->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::REPEAT) {
              auto el = RepeatComponent::from_node(child, frame->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::SAMELINE) {
              auto el = Sameline::from_node(child, frame->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::BUTTON) {
              auto el = Button::from_node(child, frame->env);
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
            if (!frame->add_element(el)) {
              LOG(WARNING) << "could not add element with id " << el->id << ", duplicate id in document";
              return nullptr;
            }
          }

          return frame;
        });
      }

      void Frame::add_usertype(sol::state_view state)
      {
        if (state[Lua::Usertypes::Ui::FRAME].get_type() == sol::type::none) {
          state.new_usertype<Frame>(Lua::Usertypes::Ui::FRAME);
        }
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
        if (this->env.has_value() && !this->eval_if(this->env.value())) {
          return;
        }

        ImVec2 vspacer = { 0, static_cast<float>(this->pos.y) };
        ImGui::Dummy(vspacer);
        ImVec2 hspacer = { static_cast<float>(this->pos.x), 0 };
        ImGui::Dummy(hspacer);
        ImGui::SameLine();
        if (ImGui::BeginChildFrame(this->imgui_id.value(), { static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) })) {
          UiComponent::render_children();
        }
        ImGui::EndChildFrame();
      }

      auto Frame::display_text() noexcept -> std::string
      {
        return std::string();
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp