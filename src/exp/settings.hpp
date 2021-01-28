#pragma once

#include "util.hpp"

namespace ExpGame
{
  class SettingsManager
  {
    using LoadResult = Result<SettingsManager, std::string>;

   public:
    SettingsManager();
    ~SettingsManager();

    static auto load(std::string_view raw) -> LoadResult;

   private:
  };
}  // namespace ExpGame
