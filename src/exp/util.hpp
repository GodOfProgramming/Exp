#pragma once

#include <optional>

namespace Exp
{
  namespace Util
  {
    template <class T, typename value_type = T::value_type>
    concept Joinable = requires(const T& t)
    {
      t.begin();
      t.end();
      t.back();
    };

    template <typename T>
    auto enumerate(T&& container)
    {
      return std::tuple{ std::begin(container), std::end(container), 0 };
    }

    struct Void
    {};

    template <typename tOk, typename tErr>
    class Result
    {
     public:
      ~Result() = default;

      constexpr static auto ok() -> Result<tOk, tErr>
      {
        return Result<tOk, tErr>();
      }

      constexpr static auto ok(tOk arg) -> Result<tOk, tErr>
      {
        Result<tOk, tErr> res;
        res.val_ok = arg;
        return res;
      }

      constexpr static auto err(tErr arg) -> Result<tOk, tErr>
      {
        Result<tOk, tErr> res;
        res.val_err = arg;
        return res;
      }

      auto ok_val() const noexcept -> tOk
      {
        return this->val_ok.value_or(tOk());
      }

      auto err_val() const noexcept -> tErr
      {
        return this->val_err.value_or(tErr());
      }

      auto is_err() const noexcept -> bool
      {
        return static_cast<bool>(this->val_err);
      }

      auto is_ok() const noexcept -> bool
      {
        return !this->is_err();
      }

      operator bool()
      {
        return this->is_ok();
      }

     private:
      std::optional<tOk> val_ok;
      std::optional<tErr> val_err;

      Result() = default;
    };

    template <Joinable T, typename value_type_t = T::value_type>
    auto join(const T& items, const char* tok = ", ") -> std::string
    {
      std::ostringstream oss;
      if (!items.empty()) {
        std::copy(items.begin(), std::prev(items.end()), std::ostream_iterator<value_type_t>(oss, tok));
        oss << items.back();
      }
      return oss.str();
    }

    template <typename T>
    auto join(const std::set<T>& items, const char* tok = ", ") -> std::string
    {
      std::vector<T> v(items.begin(), items.end());
      return join(v, tok);
    }
  }  // namespace Util
}  // namespace Exp