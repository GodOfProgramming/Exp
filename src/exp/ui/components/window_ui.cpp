#include "window_ui.hpp"

#include "exp/constants.hpp"
#include "exp/game/info.hpp"
#include "exp/render/app_window.hpp"
#include "exp/resources/scripts.hpp"
#include "frame.hpp"
#include "repeat_component.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      void WindowUi::add_usertype(sol::state& state)
      {
        state.new_usertype<WindowUi>("WindowUi", "title", &WindowUi::title, "dim", &WindowUi::dim, "pos", &WindowUi::pos);
      }

      auto WindowUi::from_node(tinyxml2::XMLNode* self) -> std::shared_ptr<UiComponent>
      {
        using Game::Info;
        using R::Scripts;
        using Render::AppWindow;

        auto self_el = self->ToElement();
        if (self_el == nullptr) {
          LOG(WARNING) << "unable to convert window to element type";
          return nullptr;
        }

        auto window = std::make_shared<WindowUi>();

        std::string if_func;
        if (UiComponent::has_attr_if(self_el, if_func)) {
          window->if_fn = if_func;
        }

        // TODO change this to a function name, similar to how text boxes work
        if (!UiComponent::has_attr_title(self_el, window->title)) {
          LOG(WARNING) << "unable to get title value of window";
          return nullptr;
        }

        UiComponent::has_attr_collapsed(self_el, window->is_collapsed);

        UiComponent::has_attr_fixed(self_el, window->is_fixed);

        auto& app_window = AppWindow::instance();
        auto size        = app_window.get_size();

        float width_percent;
        if (UiComponent::has_attr_width(self_el, width_percent)) {
          window->dim.x = static_cast<int>(size.x * width_percent / 100.0);
        } else {
          window->dim.x = static_cast<int>(size.x / 2);
        }

        float height_percent;
        if (UiComponent::has_attr_height(self_el, height_percent)) {
          window->dim.y = static_cast<int>(size.y * height_percent / 100.0);
        } else {
          window->dim.y = static_cast<int>(size.y / 2);
        }

        float x_percent;
        if (UiComponent::has_attr_x(self_el, x_percent)) {
          window->pos.x = size.x * x_percent / 100.0;
        } else {
          window->pos.x = 0;
        }

        float y_percent;
        if (UiComponent::has_attr_y(self_el, y_percent)) {
          window->pos.y = size.y * y_percent / 100.0;
        } else {
          window->pos.y = 0;
        }

        std::string script_key;
        if (UiComponent::has_attr_script(self_el, script_key)) {
          auto& scripts = Scripts::instance();

          scripts.make_script(script_key, window->script, [](sol::state& state) {
            WindowUi::add_usertype(state);
            TextBox::add_usertype(state);
            Info::add_usertype(state);
            return true;
          });

          if (!window->script.has_value()) {
            LOG(WARNING) << "unable to load script " << script_key;
            return nullptr;
          }
        }

        decltype(window->elements) potential_elements;

        for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
          std::string type = child->Value();

          if (type == UI_EL_TEXT_BOX) {
            auto el = TextBox::from_node(child, window->script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else if (type == UI_EL_REPEAT) {
            auto el = RepeatComponent::from_node(child, window->script);
            if (el) {
              potential_elements.push_back(el);
            } else {
              return nullptr;
            }
          } else if (type == UI_EL_FRAME) {
            auto el = Frame::from_node(child, *window, window->script);
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

        window->elements = std::move(potential_elements);

        return window;
      }

      auto WindowUi::width() const noexcept -> int
      {
        return this->dim.x;
      }

      auto WindowUi::height() const noexcept -> int
      {
        return this->dim.y;
      }

      void WindowUi::render()
      {
        if (!this->eval_if(this->script)) {
          return;
        }

        ImGuiWindowFlags flags = 0;
        if (this->is_fixed) {
          flags |= ImGuiWindowFlags_NoResize;
        }

        ImVec2 size{ static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) };
        ImGui::SetNextWindowSize(size);

        if (!this->initial_render) {
          ImVec2 pos{ static_cast<float>(this->pos.x), static_cast<float>(this->pos.y) };
          ImGui::SetNextWindowPos(pos);
          ImGui::SetNextWindowCollapsed(this->is_collapsed);
          this->initial_render = true;
        }

        bool is_open = true;
        if (ImGui::Begin(this->title.c_str(), &is_open, flags)) {
          auto sz   = ImGui::GetWindowSize();
          this->dim = { sz.x, sz.y };

          for (const auto& element : elements) { element->render(); }
        }
        ImGui::End();

        if (!is_open && this->script.has_value()) {
          this->enable(false);
          auto& script = this->script.value();
          auto fn      = script["OnClose"];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }
      }

      auto WindowUi::display_text() noexcept -> std::string
      {
        return this->title;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp