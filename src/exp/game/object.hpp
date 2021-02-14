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
      using ObjectMeta = Resources::ObjectMeta;
      using Uniform    = GL::Uniform;

     public:
      Object(const ObjectMeta meta);

      const ObjectMeta meta;

      void update();

      void prerender(const GL::Program& program);

      static void add_usertype(sol::state& state);

     private:
      std::optional<sol::state> script;
      std::map<std::string, Uniform> uniforms;
    };
  }  // namespace Game
}  // namespace Exp