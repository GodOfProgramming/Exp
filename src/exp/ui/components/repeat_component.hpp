#pragma once

#include "repeat_component.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class RepeatComponent: public IRepeatComponent
      {
       public:
        RepeatComponent(std::optional<sol::state>& script);
        ~RepeatComponent() final = default;

        static auto from_node(tinyxml2::XMLNode* self, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>;

        void render() final;

        auto display_text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        std::optional<sol::state>& script;
        std::string while_fn;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp