#pragma once

#include "producer.hpp"

namespace Exp
{
  namespace R
  {
    template <typename T>
    class ID: public Producable<T>
    {
     public:
      ID()
       : internal(Producer<ID>::instance().produce())
      {}

      inline ~ID()
      {
        this->release();
      }

      auto value() const noexcept -> ID::value_type final
      {
        return this->internal;
      }

      void release() final
      {
        if (!this->released) {
          auto& producer = Producer<ID>::instance();
          producer.reclaim(*this);
          this->released = true;
        }
      }

     private:
      ID::value_type internal = 0;
    };
  }  // namespace R
}  // namespace Exp