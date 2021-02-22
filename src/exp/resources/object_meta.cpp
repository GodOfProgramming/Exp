#include "object_meta.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace R
  {
    void ObjectMeta::add_usertype(sol::state_view state)
    {
      state.new_usertype<ObjectMeta>(
       Lua::Usertypes::R::META_OBJECT,
       "id",
       &ObjectMeta::id,
       "construct_fn",
       &ObjectMeta::construct_fn,
       "update_fn",
       &ObjectMeta::update_fn,
       "shader_id",
       &ObjectMeta::shader_id,
       "model_id",
       &ObjectMeta::model_id,
       "animation_id",
       &ObjectMeta::animation_id,
       "script_id",
       &ObjectMeta::script_id,
       "draw_desc",
       &ObjectMeta::drawdesc);
    }

    auto ObjectMeta::has_construct_fn(const nlohmann::json& json, std::string& value) -> bool
    {
      auto construct_json = json[JSON::Keys::GAME_OBJECT_ON_CONSTRUCT];
      if (construct_json.is_string()) {
        value = construct_json;
      } else {
        LOG(WARNING) << "object missing construct fn";
        return false;
      }

      return true;
    }

    auto ObjectMeta::has_update_fn(const nlohmann::json& json, std::string& value) -> bool
    {
      auto update_json = json[JSON::Keys::GAME_OBJECT_ON_UPDATE];
      if (update_json.is_string()) {
        value = update_json;
      } else {
        LOG(WARNING) << "object missing update fn";
        return false;
      }

      return true;
    }

    auto ObjectMeta::has_shader_id(const nlohmann::json& json, std::string& value) -> bool
    {
      auto shader_json = json[JSON::Keys::GAME_OBJECT_SHADER];
      if (shader_json.is_string()) {
        value = shader_json;
      } else {
        LOG(WARNING) << "object missing shader";
        return false;
      }

      return true;
    }

    auto ObjectMeta::has_model_id(const nlohmann::json& json, std::string& value) -> bool
    {
      auto model_json = json[JSON::Keys::GAME_OBJECT_MODEL];
      if (model_json.is_string()) {
        value = model_json;
      } else {
        LOG(WARNING) << "object missing model";
        return false;
      }

      return true;
    }

    auto ObjectMeta::has_animation_id(const nlohmann::json& json, std::string& value) -> bool
    {
      auto texture_json = json[JSON::Keys::GAME_OBJECT_ANIMATION];
      if (texture_json.is_string()) {
        value = texture_json;
      } else {
        LOG(WARNING) << "object missing texture";
        return false;
      }

      return true;
    }

    auto ObjectMeta::has_script_id(const nlohmann::json& json, std::string& value) -> bool
    {
      auto script_json = json[JSON::Keys::GAME_OBJECT_SCRIPT];
      if (script_json.is_string()) {
        value = script_json;
      } else {
        LOG(WARNING) << "object missing script";
        return false;
      }

      return true;
    }
  }  // namespace R
}  // namespace Exp
