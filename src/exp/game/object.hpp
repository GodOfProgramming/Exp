#pragma once

#include "exp/resources/object_meta.hpp"

namespace Exp
{
  namespace Game
  {
    class Object
    {
     public:
      Object(const Resources::ObjectMeta meta);

      const Resources::ObjectMeta meta;

     private:
    };
  }  // namespace Game
}  // namespace Exp