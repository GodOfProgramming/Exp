#pragma once

#include "util.hpp"

namespace ExpGame
{
  class SettingsManager
  {
    using LoadResult = Result<Void, std::string>;

    SettingsManager();

   public:
    SettingsManager(const SettingsManager&) = delete;
    SettingsManager(SettingsManager&&)      = delete;
    SettingsManager& operator=(const SettingsManager&) = delete;
    SettingsManager& operator=(SettingsManager&&) = delete;

    ~SettingsManager();

    static auto instance() -> SettingsManager&;

    auto load(std::string_view raw) -> LoadResult;

   private:
  };
}  // namespace ExpGame
