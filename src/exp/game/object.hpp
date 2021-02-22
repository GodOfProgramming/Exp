#pragma once

#include "exp/gl/program.hpp"
#include "exp/gl/uniform.hpp"
#include "exp/resources/object_meta.hpp"

namespace Exp
{
  namespace Game
  {
    class Object
    {
      using ObjectMeta    = R::ObjectMeta;
      using Uniform       = GL::Uniform;
      using AnimationMeta = R::AnimationMeta;

     public:
      Object(const ObjectMeta meta);

      const ObjectMeta meta;

      void update();

      void prerender();

      static void add_usertype(sol::state_view state);

     private:
      std::optional<sol::state> script;
      std::map<std::string, Uniform> uniforms;
    };
  }  // namespace Game
}  // namespace Exp