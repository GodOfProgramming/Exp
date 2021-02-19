#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class ITextBox: public UiComponent
      {
       public:
        virtual ~ITextBox() = default;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp