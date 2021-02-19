#pragma once

#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class IRepeatComponent: public UiComponent
      {
       public:
        virtual ~IRepeatComponent() = default;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp