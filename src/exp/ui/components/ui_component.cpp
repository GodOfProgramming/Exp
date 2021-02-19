#include "ui_component.hpp"

namespace Exp
{
  namespace Ui
  {
    void UiComponent::enable(bool value) noexcept
    {
      this->enabled = value;
    }

    auto UiComponent::is_enabled() const noexcept -> bool
    {
      return this->enabled;
    }
  }  // namespace Ui
}  // namespace Exp
