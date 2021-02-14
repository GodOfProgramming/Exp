#pragma once

#include "exp/resources/object_meta.hpp"

namespace Exp
{
  namespace Game
  {
    class Object
    {
      using ObjectMeta = Resources::ObjectMeta;

     public:
      Object(const ObjectMeta meta);

      const ObjectMeta meta;

      void update();

      static void add_usertype(sol::state& state);

     private:
      std::optional<sol::state> script;
    };
  }  // namespace Game
}  // namespace Exp