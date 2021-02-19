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

      virtual auto text() noexcept -> std::string = 0;

      void enable(bool) noexcept;

      auto is_enabled() const noexcept -> bool;

     private:
      bool enabled = false;
    };
  }  // namespace Ui
}  // namespace Exp