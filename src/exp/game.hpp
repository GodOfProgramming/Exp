#pragma once
#include "resources.hpp"

namespace ExpGame
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
}  // namespace ExpGame
