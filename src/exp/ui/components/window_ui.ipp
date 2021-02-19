#pragma once

#include "container.ipp"

namespace Exp
{
  namespace Ui
  {
    namespace Components
    {
      class IWindowUi: public Container
      {
       public:
        virtual ~IWindowUi() = default;
      };
    }  // namespace Components
  }    // namespace Ui
}  // namespace Exp