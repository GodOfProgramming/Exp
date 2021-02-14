#pragma once

namespace Exp
{
  namespace Ui
  {
    class UiComponent
    {
     public:
      virtual ~UiComponent() = default;

      virtual void render() = 0;
    };
  }  // namespace Ui
}  // namespace Exp