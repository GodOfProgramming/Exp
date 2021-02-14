#pragma once

#include "exp/util.hpp"

namespace Exp
{
  namespace IO
  {
    class File
    {
      using LoadResult = Util::Result<File, std::string>;

     public:
      File() = default;
      File(std::string data);

      auto save(std::filesystem::path p) -> bool;

      static auto load(std::filesystem::path filename) -> LoadResult;

      static auto dirname(std::filesystem::path path) -> std::filesystem::path;

      std::string data;

     private:
    };
  }  // namespace IO
}  // namespace Exp