#pragma once

#include "producable.hpp"

namespace Exp
{
  namespace R
  {
    template <class T>
    class Producer
    {
      using value_type = Producable<typename T::value_type>::value_type;

      Producer() = default;

     public:
      Producer(const Producer&) = delete;
      Producer(Producer&&)      = delete;
      auto operator=(const Producer&) -> Producer& = delete;
      auto operator=(Producer&&) -> Producer& = delete;

      static auto instance() noexcept -> Producer<T>&
      {
        static Producer<T> producer;
        return producer;
      }

      auto produce() -> value_type
      {
        if (this->free_items.empty()) {
          return ++next_item;
        } else {
          auto first = this->free_items.begin();
          this->free_items.erase(first);
          return *first;
        }
      }

      auto reclaim(const Producable<value_type>& item)
      {
        this->free_items.emplace(item.value());
      }

     private:
      value_type next_item;
      std::set<value_type> free_items;
    };
  }  // namespace R
}  // namespace Exp
