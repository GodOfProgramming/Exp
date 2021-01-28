#include <optional>

namespace ExpGame
{
  template <typename T>
  class Result
  {
   public:
    ~Result() = default;

    static auto ok() -> Result<T>
    {
      return Result<T>();
    }

    static auto err(T&& arg) -> Result<T>
    {
      return Result(std::move(arg));
    }

    auto is_err() -> bool
    {
      return static_cast<bool>(this->contents);
    }

    auto err() -> T
    {
      return this->contents.value_or(T());
    }

   private:
    Result() = default;

    Result(T&& arg)
     : contents(std::move(arg))
    {}

    const std::optional<T> contents;
  };
}  // namespace ExpGame