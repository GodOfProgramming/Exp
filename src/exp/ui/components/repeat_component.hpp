#pragma once

#include "exp/ui/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class RepeatComponent: public UiComponent
      {
       public:
        RepeatComponent(std::optional<sol::state>& script, std::string fn);

        static auto from_node(tinyxml2::XMLNode* child, std::optional<sol::state>& script) -> std::shared_ptr<UiComponent>;

        void render() final;

        auto text() noexcept -> std::string final;

       private:
        std::vector<std::shared_ptr<UiComponent>> elements;
        std::optional<sol::state>& opt_script;

        sol::state& script;
        std::string function;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp