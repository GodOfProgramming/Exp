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

      static void load(std::filesystem::path path, std::function<void(std::string_view)> fn);

      static auto dirname(std::filesystem::path path) -> std::filesystem::path;

      std::string data;

     private:
    };
  }  // namespace IO
}  // namespace Exp