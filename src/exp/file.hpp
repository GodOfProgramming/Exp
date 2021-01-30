#pragma once

#include "util.hpp"

namespace ExpGame
{
  class File
  {
    using LoadResult = Result<File, std::string>;

   public:
    File() = default;
    File(std::string data);

    auto save(std::filesystem::path p) -> bool;

    static auto load(std::filesystem::path filename) -> LoadResult;

    std::string data;

   private:
  };
}  // namespace ExpGame