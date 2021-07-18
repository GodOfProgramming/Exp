#pragma once

namespace Exp
{
  namespace R
  {
    template <typename T>
    struct PassthroughAccessor
    {
      using Type = T;

      static constexpr void set(T& t, const Type& product)
      {
        t = product;
      }

      static constexpr auto get(const T& t) -> const Type&
      {
        return t;
      }
    };

    template <typename T>
    struct StandardIncrementer
    {
      using Type = T;

      static constexpr void increment(Type& t)
      {
        t++;
      }
    };

    template <class T, class F = PassthroughAccessor<T>, class I = StandardIncrementer<T>, typename V = typename F::Type>
    class Producer
    {
      static_assert(std::is_same_v<typename F::Type, typename I::Type>, "accessor and incrementer types are not in sync");

     public:
      Producer()                = default;
      Producer(const Producer&) = delete;
      Producer(Producer&&)      = default;
      auto operator=(const Producer&) -> Producer& = delete;
      auto operator=(Producer&&) -> Producer& = default;

      static auto instance() noexcept -> Producer&
      {
        static Producer producer;
        return producer;
      }

      auto produce() -> T
      {
        V product;
        if (this->reclaimed_products.empty()) {
          product = I::Increment(this->next_item);
        } else {
          auto first = this->reclaimed_products.begin();
          this->reclaimed_products.erase(first);
          product = *first;
        }

        T t{ this };
        F::set(t, product);
        return t;
      }

      auto reclaim(const T& product)
      {
        this->reclaimed_products.emplace(F::get(product));
      }

     private:
      V next_item;
      std::set<V> reclaimed_products;
    };
  }  // namespace R
}  // namespace Exp
