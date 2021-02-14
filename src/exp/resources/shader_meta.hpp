#pragma once

namespace Exp
{
  namespace Resources
  {
    struct ShaderMeta
    {
      std::string file;
      std::string source;
      std::string error;
      bool present = false;
    };
  }  // namespace Resources
}  // namespace Exp