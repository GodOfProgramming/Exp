#pragma once

namespace Exp
{
  namespace GL
  {
    struct Error
    {
      std::string desc;
      std::map<std::string, std::map<int, std::size_t>> occurrences;
    };
  }  // namespace GL
}  // namespace Exp
