#pragma once

#include "util.hpp"

namespace ExpGame
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

    void iterate_dir_with_namespace(
     std::filesystem::path dir,
     std::string nspace,
     const std::function<void(const std::filesystem::path, const std::string&)> fn);
  }  // namespace IO
}  // namespace ExpGame