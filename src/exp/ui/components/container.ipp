#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class Container: public UiComponent
      {
       public:
        virtual auto width() const noexcept -> int  = 0;
        virtual auto height() const noexcept -> int = 0;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp