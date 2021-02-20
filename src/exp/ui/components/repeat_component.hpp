#pragma once

#include "exp/ui/components/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class RepeatComponent: public UiComponent
      {
       public:
        RepeatComponent(std::optional<sol::state_view> script);
        ~RepeatComponent() final;

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state_view> script) -> std::shared_ptr<UiComponent>;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::string while_fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp