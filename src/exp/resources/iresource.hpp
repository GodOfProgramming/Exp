#pragma once

namespace Exp
{
  namespace R
  {
    class IResource
    {
     public:
      virtual void release() = 0;
    };
  }  // namespace R
}  // namespace Exp