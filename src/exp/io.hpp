#pragma once

#include "util.hpp"

namespace Exp
{
  namespace IO
  {
    void iterate_dir_with_namespace(
     std::filesystem::path dir, std::string nspace, const std::function<void(const std::filesystem::path, const std::string&)> fn);
  }  // namespace IO
}  // namespace Exp