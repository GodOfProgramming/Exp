#pragma once

#include "producer.hpp"

namespace Exp
{
  namespace R
  {
    template <typename T, typename V>
    struct IDFieldSelector
    {
      using field_t = V;

      static void set(T& t, const field_t& f)
      {
        t.emplace(f);
      }

      static auto get(const T& t) -> const field_t
      {
        return t.value();
      }
    };

    template <typename T>
    class ID
    {
     public:
      using value_type = T;

      using producer_t = Producer<ID, IDFieldSelector<ID, value_type>>;

      ID() = default;
      ID(value_type v)
       : internal(v)
      {}

      auto value() const noexcept -> value_type
      {
        return this->internal;
      }

      void emplace(const value_type& v)
      {
        this->internal = v;
      }

      void release() noexcept
      {
        if (!this->released) {
          auto& producer = producer_t::instance();
          producer.reclaim(*this);
          this->released = true;
          this->internal = 0;
        }
      }

     private:
      bool released       = false;
      value_type internal = 0;
    };
  }  // namespace R
}  // namespace Exp