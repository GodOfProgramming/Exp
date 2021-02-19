#pragma once

#include "window_ui.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class WindowUi: public IWindowUi
      {
       public:
        WindowUi()        = default;
        ~WindowUi() final = default;

        static auto from_node(tinyxml2::XMLNode* self) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state& state);

        auto width() const noexcept -> int;

        auto height() const noexcept -> int;

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        std::optional<sol::state> script;

        std::string title;
        glm::ivec2 dim      = {};
        glm::ivec2 pos      = {};
        bool initial_render = false;
        bool is_collapsed   = false;
        bool is_fixed       = false;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp