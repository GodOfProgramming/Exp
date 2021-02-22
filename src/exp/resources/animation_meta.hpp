#pragma once

#include "texture_meta.hpp"

namespace Exp
{
  namespace R
  {
    struct AnimationMeta
    {
      using ActionMap = std::map<std::string, std::vector<std::size_t>>;
      std::size_t width;
      std::size_t height;

      std::shared_ptr<TextureMeta> texture;
      ActionMap action_map;

      static void add_usertype(sol::state_view state);

      static auto has_width(const nlohmann::json& json, std::size_t& value) -> bool;
      static auto has_height(const nlohmann::json& json, std::size_t& value) -> bool;
      static auto has_texture_id(const nlohmann::json& json, std::string& value) -> bool;
      static auto parse_actions(const nlohmann::json& json, ActionMap& actions) -> bool;

      auto next(std::string action, std::size_t) -> std::size_t;
      auto uv(std::string action, std::size_t current) -> glm::vec2;
    };
  }  // namespace R
}  // namespace Exp