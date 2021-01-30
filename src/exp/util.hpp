#pragma once

#include <optional>

namespace ExpGame
{
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

  template <typename... Args>
  static auto to_string(Args&&... args) noexcept -> std::string
  {
    std::stringstream ss;
    (((ss) << std::forward<Args>(args)), ...);
    return ss.str();
  }
}  // namespace ExpGame