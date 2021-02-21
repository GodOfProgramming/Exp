#pragma once

#include "exp/ui/components/container.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class WindowUi: public Container
      {
       public:
        WindowUi();
        ~WindowUi() final;

        static auto from_node(tinyxml2::XMLNode* self) -> std::shared_ptr<UiComponent>;

        static void add_usertype(sol::state_view state);

        auto width() const noexcept -> int;

        auto height() const noexcept -> int;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::optional<sol::state> doc_script;

        std::string title;
        bool initial_render = false;
        bool is_collapsed   = false;
        bool is_fixed       = false;

        std::optional<std::string> onparse_fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp