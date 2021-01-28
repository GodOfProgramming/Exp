#pragma once

#include "util.hpp"

namespace ExpGame
{
  class SettingsManager
  {
    using LoadResult = Result<std::string>;

   public:
    SettingsManager();
    ~SettingsManager();

    auto load(std::string_view raw) -> LoadResult;

   private:
  };
}  // namespace ExpGame
