#pragma once

#include "exp/ui/ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class TextBox: public UiComponent
      {
       public:
        TextBox(std::optional<sol::state>& script, std::string fn, std::string text);

        void render() final;

        static void add_usertype(sol::state& state);

       private:
        std::optional<sol::state>& script;
        std::string function;
        std::string text;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp