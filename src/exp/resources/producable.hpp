#pragma once

namespace Exp
{
  namespace R
  {
    template <typename T>
    class Producable
    {
     public:
      using value_type = T;

      virtual ~Producable() = default;

      virtual auto value() const noexcept -> T = 0;

      virtual void release() = 0;

     protected:
      bool released = false;
    };
  }  // namespace R
}  // namespace Exp