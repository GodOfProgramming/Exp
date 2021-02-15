#pragma once

#include "exp/gl/draw_description.hpp"
#include "exp/gl/program.hpp"
#include "model_meta.hpp"

namespace Exp
{
  namespace Resources
  {
    struct ObjectMeta
    {
      std::string id;
      std::shared_ptr<GL::Program> shader;
      std::shared_ptr<ModelMeta> model;
      GL::DrawDescription drawdesc;
      std::optional<std::string> script_id;

      static void add_usertype(sol::state& state);
    };
  }  // namespace Resources
}  // namespace Exp