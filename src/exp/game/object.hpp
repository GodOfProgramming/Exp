#pragma once

#include "exp/gl/program.hpp"
#include "exp/gl/uniform.hpp"
#include "exp/resources/id.hpp"
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
      Object(const ObjectMeta meta, glm::vec3 pos);
      ~Object();

      static void add_usertype(sol::state_view state);

      void update();

      void prerender();

      auto id() const noexcept -> std::size_t;

      auto location() const noexcept -> glm::vec3;

      const ObjectMeta meta;

     private:
      std::optional<sol::environment> env;
      std::map<std::string, std::shared_ptr<Uniform>> uniforms;
      glm::vec3 pos;
      sol::table data;
      R::ID<std::size_t> obj_id;
    };
  }  // namespace Game
}  // namespace Exp