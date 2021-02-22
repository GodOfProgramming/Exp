#pragma once

#include "animation_meta.hpp"
#include "exp/gl/program.hpp"
#include "exp/render/draw_description.hpp"
#include "model_meta.hpp"

namespace Exp
{
  namespace R
  {
    struct ObjectMeta
    {
      std::string id;
      std::string construct_fn;
      std::string update_fn;
      std::string shader_id;
      std::string model_id;
      std::string animation_id;
      std::string script_id;

      std::shared_ptr<GL::Program> shader;
      std::shared_ptr<ModelMeta> model;
      std::shared_ptr<AnimationMeta> animation;

      Render::DrawDescription drawdesc;

      static void add_usertype(sol::state_view state);

      static auto has_construct_fn(const nlohmann::json& json, std::string& value) -> bool;
      static auto has_update_fn(const nlohmann::json& json, std::string& value) -> bool;
      static auto has_shader_id(const nlohmann::json& json, std::string& value) -> bool;
      static auto has_model_id(const nlohmann::json& json, std::string& value) -> bool;
      static auto has_animation_id(const nlohmann::json& json, std::string& value) -> bool;
      static auto has_script_id(const nlohmann::json& json, std::string& value) -> bool;
    };
  }  // namespace R
}  // namespace Exp