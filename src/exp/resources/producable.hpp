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

      ~Producable()
      {
        this->release();
      }

      virtual auto value() const noexcept -> T = 0;

      void release()
      {
        if (!released) {
          this->release_impl();
        }
      }

     protected:
      virtual void release_impl() = 0;

     private:
      bool released = false;
    };
  }  // namespace R
}  // namespace Exp