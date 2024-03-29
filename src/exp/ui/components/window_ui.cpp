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

// draw basic text using
// NoTitleBar|NoResize|NoMove|NoScrollbar|NoSavedSettings|NoInputs
// https://gamedev.stackexchange.com/questions/120964/how-can-i-use-imgui-to-render-simple-text-instead-of-using-stb-truetype-directly

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

      void WindowUi::add_usertype(sol::state_view state)
      {
        if (state[Lua::Usertypes::Ui::WINDOW_UI].get_type() == sol::type::none) {
          state.new_usertype<WindowUi>(Lua::Usertypes::Ui::WINDOW_UI, "title", &WindowUi::title, "dim", &WindowUi::dim, "pos", &WindowUi::pos);
        }
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

          std::string onparse_func;
          std::optional<std::string> onparse_fn;
          if (UiComponent::has_attr_onparse(self, onparse_func)) {
            onparse_fn = onparse_func;
          }

          std::string script_key;
          if (UiComponent::has_attr_script(self, script_key)) {
            auto& scripts = Scripts::instance();

            sol::environment env;
            if (!scripts.create_env(script_key, env)) {
              LOG(WARNING) << "unable to load script " << script_key;
              return nullptr;
            }

            window->env = std::move(env);
          }

          std::vector<std::shared_ptr<UiComponent>> potential_elements;

          for (auto child = self->FirstChild(); child != nullptr; child = child->NextSibling()) {
            std::string type = child->Value();

            if (type == El::TEXT_BOX) {
              auto el = TextBox::from_node(child, window->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::REPEAT) {
              auto el = RepeatComponent::from_node(child, window->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::FRAME) {
              auto el = Frame::from_node(child, window->env, *window);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::SAMELINE) {
              auto el = Sameline::from_node(child, window->env);
              if (el) {
                potential_elements.push_back(el);
              } else {
                return nullptr;
              }
            } else if (type == El::BUTTON) {
              auto el = Button::from_node(child, window->env);
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

          if (window->env.has_value() && onparse_fn.has_value()) {
            auto& env  = window->env.value();
            auto& func = onparse_fn.value();
            auto fn    = env[func];
            if (fn.get_type() == sol::type::function) {
              fn.call(window);
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
        if (this->env.has_value() && !this->eval_if(this->env.value())) {
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

        if (!is_open && this->env.has_value()) {
          this->enable(false);
          auto& lua = this->env.value();
          auto fn   = lua["OnClose"];
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