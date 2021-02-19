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

        auto window = std::make_shared<WindowUi>();

        auto self_el = self->ToElement();
        if (self_el == nullptr) {
          LOG(WARNING) << "unable to convert window to element type";
          return nullptr;
        }

        // TODO change this to a function name, similar to how text boxes work
        {
          auto title_attr = self_el->FindAttribute("title");

          if (title_attr == nullptr) {
            LOG(WARNING) << "unable to get title value of window";
            return nullptr;
          }

          window->title = title_attr->Value();
        }

        {
          auto collapsed_attr = self_el->FindAttribute("collapsed");

          if (collapsed_attr != nullptr) {
            bool collapsed = false;
            auto res       = collapsed_attr->QueryBoolValue(&collapsed);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse width: " << res;
              return nullptr;
            }
            window->is_collapsed = collapsed;
          }
        }

        {
          auto fixed_attr = self_el->FindAttribute("fixed");

          if (fixed_attr != nullptr) {
            bool fixed = false;
            auto res   = fixed_attr->QueryBoolValue(&fixed);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse width: " << res;
              return nullptr;
            }
            window->is_fixed = fixed;
          }
        }

        auto& app_window = AppWindow::instance();
        auto size        = app_window.get_size();

        {
          auto width_attr = self_el->FindAttribute("width");
          if (width_attr == nullptr) {
            window->dim.x = size.x / 2;
          } else {
            float percent{ 0.0 };
            auto res = width_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse width: " << res;
              return nullptr;
            }
            window->dim.x = size.x * percent / 100.0;
          }
        }

        {
          auto height_attr = self_el->FindAttribute("height");
          if (height_attr == nullptr) {
            window->dim.y = size.y / 2;
          } else {
            float percent{ 0.0 };
            auto res = height_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse height: " << res;
              return nullptr;
            }
            window->dim.y = size.y * percent / 100.0;
          }
        }

        {
          auto x_attr = self_el->FindAttribute("x");
          if (x_attr == nullptr) {
            window->pos.x = 0;
          } else {
            float percent{ 0.0 };
            auto res = x_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse x pos: " << res;
              return nullptr;
            }
            window->pos.x = size.x * percent / 100.0;
          }
        }

        {
          auto y_attr = self_el->FindAttribute("y");
          if (y_attr == nullptr) {
            window->pos.y = 0;
          } else {
            float percent{ 0.0 };
            auto res = y_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse y pos: " << res;
              return nullptr;
            }
            window->pos.y = size.y * percent / 100.0;
          }
        }

        {
          auto script_attr = self_el->FindAttribute("script");
          if (script_attr != nullptr) {
            std::string script_key = script_attr->Value();
            auto& scripts          = Scripts::instance();

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