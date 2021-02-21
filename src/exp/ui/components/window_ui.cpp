#include "window_ui.hpp"

#include "button.hpp"
#include "exp/constants.hpp"
#include "exp/game/info.hpp"
#include "exp/render/app_window.hpp"
#include "exp/resources/scripts.hpp"
#include "frame.hpp"
#include "repeat_component.hpp"
#include "sameline.hpp"
#include "text_box.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      WindowUi::WindowUi()
       : Container(std::nullopt)
      {}

      WindowUi::~WindowUi()
      {
        Container::release();
      }

      void WindowUi::add_usertype(sol::state_view& state)
      {
        state.new_usertype<WindowUi>("WindowUi", "title", &WindowUi::title, "dim", &WindowUi::dim, "pos", &WindowUi::pos);
      }

      auto WindowUi::from_node(tinyxml2::XMLNode* self) -> std::shared_ptr<UiComponent>
      {
        return UiComponent::unwrap_node(self, [](tinyxml2::XMLElement* self) -> std::shared_ptr<UiComponent> {
          using Game::Info;
          using R::Scripts;
          using Render::AppWindow;

          std::shared_ptr<WindowUi> window(new WindowUi());

          auto& app_window = AppWindow::instance();
          auto size        = app_window.get_size();

          if (!Container::from_node(self, window, size)) {
            return nullptr;
          }

          // TODO change this to a function name, similar to how text boxes work
          if (!UiComponent::has_attr_title(self, window->title)) {
            LOG(WARNING) << "unable to get title value of window";
            return nullptr;
          }

          UiComponent::has_attr_collapsed(self, window->is_collapsed);

          UiComponent::has_attr_fixed(self, window->is_fixed);

          std::string script_key;
          if (UiComponent::has_attr_script(self, script_key)) {
            auto& scripts = Scripts::instance();

            sol::state lua;
            if (!scripts.make_script(script_key, lua, [window](sol::state_view& state) {
                  WindowUi::add_usertype(state);
                  TextBox::add_usertype(state);
                  Info::add_usertype(state);
                  state.set("window", window);
                  return true;
                })) {
              LOG(WARNING) << "unable to load script " << script_key;
              return nullptr;
            }

            window->doc_script = std::move(lua);
            window->script     = window->doc_script.value();
          }

          std::vector<std::shared_ptr<UiComponent>> potential_elements;

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
              auto el = Frame::from_node(child, window->script, *window);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == UI_EL_SAMELINE) {
              auto el = Sameline::from_node(child, window->script);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == UI_EL_BUTTON) {
              auto el = Button::from_node(child, window->script);
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
            if (!window->add_element(el)) {
              LOG(WARNING) << "could not add element with id " << el->id << ", duplicate id in document";
              return nullptr;
            }
          }

          return window;
        });
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
        if (this->script.has_value() && !this->eval_if(this->script.value())) {
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

          UiComponent::render_children();
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