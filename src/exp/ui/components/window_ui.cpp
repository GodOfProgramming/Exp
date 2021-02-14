#include "window_ui.hpp"

#include "exp/game/info.hpp"
#include "exp/render/app_window.hpp"
#include "exp/resources/scripts.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      WindowUi::WindowUi(std::string t)
       : title(t)
       , dim({ 0, 0 })
       , pos({ 0, 0 })
       , hide(false)
       , initial_render(false)
       , is_collapsed(false)
      {}

      void WindowUi::render()
      {
        if (this->hide) {
          return;
        }

        ImGuiWindowFlags flags = 0;
        flags |= ImGuiWindowFlags_NoResize;

        ImVec2 size{ static_cast<float>(this->dim.x), static_cast<float>(this->dim.y) };
        ImGui::SetNextWindowSize(size);

        if (!this->initial_render) {
          ImVec2 pos{ static_cast<float>(this->pos.x), static_cast<float>(this->pos.y) };
          ImGui::SetNextWindowPos(pos);
          ImGui::SetNextWindowCollapsed(this->is_collapsed);
          this->initial_render = true;
        }

        bool is_open = true;
        ImGui::Begin(this->title.c_str(), &is_open, flags);

        for (const auto& element : elements) { element->render(); }

        ImGui::End();

        if (!is_open && this->lua.has_value()) {
          this->hide   = true;
          auto& script = this->lua.value();
          auto fn      = script["on_close"];
          if (fn.get_type() == sol::type::function) {
            fn.call(this);
          }
        }
      }

      auto WindowUi::parse(tinyxml2::XMLNode* self) -> bool
      {
        using Game::Info;
        using Render::AppWindow;
        using Resources::Scripts;

        auto element = self->ToElement();
        if (element == nullptr) {
          LOG(WARNING) << "unable to convert window to element type";
          return false;
        }

        auto title_attr = element->FindAttribute("title");

        if (title_attr == nullptr) {
          LOG(WARNING) << "unable to get title value of window";
          return false;
        }

        auto collapsed_attr = element->FindAttribute("collapsed");

        if (collapsed_attr != nullptr) {
          bool collapsed = false;
          auto res       = collapsed_attr->QueryBoolValue(&collapsed);
          if (res != tinyxml2::XML_SUCCESS) {
            LOG(WARNING) << "unable to parse width: " << res;
            return false;
          }
          this->is_collapsed = collapsed;
        }

        this->title = title_attr->Value();

        auto& window = AppWindow::instance();
        auto size    = window.get_size();

        {
          auto width_attr = element->FindAttribute("width");
          if (width_attr == nullptr) {
            this->dim.x = size.x / 2;
          } else {
            float percent{ 0.0 };
            auto res = width_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse width: " << res;
              return false;
            }
            this->dim.x = size.x * percent / 100.0;
          }
        }

        {
          auto height_attr = element->FindAttribute("height");
          if (height_attr == nullptr) {
            this->dim.y = size.y / 2;
          } else {
            float percent{ 0.0 };
            auto res = height_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse height: " << res;
              return false;
            }
            this->dim.y = size.y * percent / 100.0;
          }
        }

        {
          auto x_attr = element->FindAttribute("x");
          if (x_attr == nullptr) {
            this->pos.x = 0;
          } else {
            float percent{ 0.0 };
            auto res = x_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse x pos: " << res;
              return false;
            }
            this->pos.x = size.x * percent / 100.0;
          }
        }

        {
          auto y_attr = element->FindAttribute("y");
          if (y_attr == nullptr) {
            this->pos.y = 0;
          } else {
            float percent{ 0.0 };
            auto res = y_attr->QueryFloatValue(&percent);
            if (res != tinyxml2::XML_SUCCESS) {
              LOG(WARNING) << "unable to parse y pos: " << res;
              return false;
            }
            this->pos.y = size.y * percent / 100.0;
          }
        }

        {
          auto script_attr = element->FindAttribute("script");
          if (script_attr != nullptr) {
            std::string script_key = script_attr->Value();
            auto& scripts          = Scripts::instance();
            auto script_iter       = scripts.find(script_key);
            if (script_iter == scripts.end()) {
              LOG(WARNING) << "unable to load script " << script_key;
              return false;
            }

            auto& script = script_iter->second;

            sol::state state;
            auto res = state.load(script.src);
            if (!res.valid()) {
              LOG(WARNING) << "script " << script_key << " is invalid";
              return false;
            }

            auto exec_res = res();

            if (!exec_res.valid()) {
              LOG(WARNING) << "script " << script_key << " was not able to run";
              return false;
            }

            state.open_libraries(sol::lib::base, sol::lib::string);

            state.new_usertype<glm::ivec2>("ivec2", "x", &glm::ivec2::x, "y", &glm::ivec2::y);
            state.new_usertype<WindowUi>("WindowUi", "title", &WindowUi::title, "dim", &WindowUi::dim, "pos", &WindowUi::pos);
            state.new_usertype<Info>("GameInfo", "fps", &Info::fps, "frames", &Info::frames);

            state.set("game_info", &Info::instance());

            this->lua = std::move(state);
          }
        }

        return true;
      }
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp