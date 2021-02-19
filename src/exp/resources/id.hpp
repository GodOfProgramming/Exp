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

      ~ID() = default;

      auto value() const noexcept -> ID::value_type final
      {
        return this->internal;
      }

     protected:
      void release_impl() final
      {
        auto& producer = Producer<ID>::instance();
        producer.reclaim(*this);
      }

     private:
      ID::value_type internal = 0;
    };
  }  // namespace R
}  // namespace Exp