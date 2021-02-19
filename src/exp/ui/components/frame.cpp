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
      {
        auto& producer = R::ID<ImGuiID>::producer_t::instance();
        this->id       = producer.produce();
      }

      Frame::~Frame()
      {
        this->id.release();
      }

      auto Frame::from_node(tinyxml2::XMLNode* self, const Container& container, std::optional<sol::state>& parent_script) -> std::shared_ptr<UiComponent>
      {
        using Game::Info;
        using R::Scripts;

        auto self_el = self->ToElement();
        if (self_el == nullptr) {
          LOG(WARNING) << "unable to convert frame to element type";
          return nullptr;
        }

        auto frame = std::make_shared<Frame>(container, parent_script);

        std::string fn;
        if (UiComponent::has_attr_if(self_el, fn)) {
          frame->if_fn = fn;
        }

        glm::ivec2 size = { container.width(), container.height() };

        float width_percent;
        if (UiComponent::has_attr_width(self_el, width_percent)) {
          frame->dim.x = static_cast<int>(size.x * width_percent / 100.0);
        } else {
          frame->dim.x = static_cast<int>(size.x);
        }

        float height_percent;
        if (UiComponent::has_attr_height(self_el, height_percent)) {
          frame->dim.y = static_cast<int>(size.y * height_percent / 100.0);
        } else {
          frame->dim.y = static_cast<int>(size.y / 2);
        }

        float x_percent;
        if (UiComponent::has_attr_x(self_el, x_percent)) {
          frame->pos.x = size.x * x_percent / 100.0;
        } else {
          frame->pos.x = 0;
        }

        float y_percent;
        if (UiComponent::has_attr_y(self_el, y_percent)) {
          frame->pos.y = size.y * y_percent / 100.0;
        } else {
          frame->pos.y = 0;
        }

        std::string script_key;
        if (UiComponent::has_attr_script(self_el, script_key)) {
          auto& scripts = Scripts::instance();

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
        auto& luaopt = this->script.has_value() ? this->script : this->parent_script;
        if (!this->eval_if(luaopt)) {
          return;
        }

        ImVec2 vspacer = { 0, static_cast<float>(this->pos.y) };
        ImGui::Dummy(vspacer);
        ImVec2 hspacer = { static_cast<float>(this->pos.x), 0 };
        ImGui::Dummy(hspacer);
        ImGui::SameLine();
        if (ImGui::BeginChildFrame(id.value(), { static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) })) {
          for (const auto& element : elements) { element->render(); }
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