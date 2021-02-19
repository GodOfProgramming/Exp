#pragma once

namespace Exp
{
  namespace R
  {
    template <typename T>
    struct PassthroughFieldSelector
    {
      using field_t = T;

      static constexpr void set(T& t, const field_t& product)
      {
        t = product;
      }

      static constexpr auto get(const T& t) -> const field_t&
      {
        return t;
      }
    };

    template <class T, class F = PassthroughFieldSelector<T>, typename V = typename F::field_t>
    class Producer
    {
      Producer() = default;

     public:
      Producer(const Producer&) = delete;
      Producer(Producer&&)      = delete;
      auto operator=(const Producer&) -> Producer& = delete;
      auto operator=(Producer&&) -> Producer& = delete;

      static auto instance() noexcept -> Producer&
      {
        static Producer producer;
        return producer;
      }

      auto produce() -> T
      {
        V product;
        if (this->reclaimed_products.empty()) {
          product = ++next_item;
        } else {
          auto first = this->reclaimed_products.begin();
          this->reclaimed_products.erase(first);
          product = *first;
        }

        T t;
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
