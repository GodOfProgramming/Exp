#pragma once

namespace ExpGame
{
  class UiManager
  {
    UiManager() = default;

   public:
    UiManager(const UiManager&) = delete;
    UiManager(UiManager&&)      = delete;
    auto operator=(const UiManager&) -> UiManager& = delete;
    auto operator=(UiManager&&) -> UiManager& = delete;

    static auto instance() -> UiManager&;

   private:
  };
}  // namespace ExpGame
