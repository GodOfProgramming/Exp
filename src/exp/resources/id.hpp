#pragma once

#include "producer.hpp"

namespace Exp
{
  namespace R
  {
    template <typename T, typename V>
    struct IDAccessor
    {
      using Type = V;

      static void set(T& t, const Type& f)
      {
        t.emplace(f);
      }

      static auto get(const T& t) -> const Type
      {
        return t.value();
      }
    };

    template <typename T>
    class ID
    {
      static_assert(std::is_integral_v<T>, "T is not integral type");

     public:
      using ValueType  = T;
      using IDProducer = Producer<ID<T>, IDAccessor<ID<T>, ValueType>>;

      ID() = default;
      explicit ID(IDProducer* p)
       : producer{ p }
      {}

      explicit ID(ValueType v)
       : internal(v)
      {}

      auto value() const noexcept -> ValueType
      {
        return this->internal;
      }

      void emplace(const ValueType& v)
      {
        this->internal = v;
      }

      void release() noexcept
      {
        if (!this->released && this->producer) {
          this->producer->reclaim(*this);
          this->released = true;
          this->internal = 0;
        }
      }

     private:
      bool released        = false;
      IDProducer* producer = nullptr;
      ValueType internal   = 0;
    };
  }  // namespace R
}  // namespace Exp