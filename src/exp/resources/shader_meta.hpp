#pragma once

namespace Exp
{
  namespace R
  {
    struct ShaderMeta
    {
      std::string file;
      std::string source;
      std::string error;
      bool present = false;
    };
  }  // namespace R
}  // namespace Exp