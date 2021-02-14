#pragma once

namespace Exp
{
  namespace Resources
  {
    class IResource
    {
     public:
      virtual void release() = 0;
    };
  }  // namespace Resources
}  // namespace Exp