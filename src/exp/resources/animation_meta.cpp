#include "animation_meta.hpp"

#include "exp/constants.hpp"

namespace Exp
{
  namespace R
  {
    void AnimationMeta::add_usertype(sol::state_view state)
    {
      state.new_usertype<AnimationMeta>(
       Lua::Usertypes::R::META_ANIMATION, "ratio", &AnimationMeta::ratio, "next", &AnimationMeta::next, "uv", &AnimationMeta::uv);
    }

    auto AnimationMeta::ratio() -> glm::vec2
    {
      float xfract = 1.0 / this->width;
      float yfract = 1.0 / this->height;
      return glm::vec2{ xfract, yfract };
    }

    auto AnimationMeta::next(std::string action, std::size_t current) -> std::size_t
    {
      current++;
      auto& actions = this->action_map[action];
      if (actions.begin() + current >= actions.end()) {
        return 0;
      } else {
        return current;
      }
    }

    auto AnimationMeta::uv(std::string action, std::size_t current) -> glm::vec2
    {
      auto& actions   = this->action_map[action];
      auto frame      = actions[current];
      auto fract      = this->ratio();
      std::size_t row = frame / this->width;
      std::size_t col = frame % this->width;
      return glm::vec2(col * fract.x, row * fract.y);
    }

    auto AnimationMeta::has_width(const nlohmann::json& json, std::size_t& value) -> bool
    {
      auto width_json = json[JSON::Keys::ANIMATION_WIDTH];
      if (width_json.is_number_unsigned()) {
        value = width_json;
      } else {
        LOG(WARNING) << "width is not number";
        return false;
      }

      return true;
    }

    auto AnimationMeta::has_height(const nlohmann::json& json, std::size_t& value) -> bool
    {
      auto height_json = json[JSON::Keys::ANIMATION_HEIGHT];
      if (height_json.is_number_unsigned()) {
        value = height_json;
      } else {
        LOG(WARNING) << "height is not number";
        return false;
      }

      return true;
    }

    auto AnimationMeta::has_texture_id(const nlohmann::json& json, std::string& value) -> bool
    {
      auto texture_json = json[JSON::Keys::ANIMATION_TEXTURE];
      if (texture_json.is_string()) {
        value = texture_json;
      } else {
        LOG(WARNING) << "object missing texture";
        return false;
      }

      return true;
    }

    auto AnimationMeta::parse_actions(const nlohmann::json& json, ActionMap& actions) -> bool
    {
      if (json.contains(JSON::Keys::ANIMATION_ACTIONS)) {
        auto action_json = json[JSON::Keys::ANIMATION_ACTIONS];

        if (!action_json.is_null() && action_json.is_object()) {
          for (const auto item : action_json.items()) {
            auto action = item.key();
            auto obj    = item.value();

            LOG(INFO) << "registering action " << action;

            if (!obj.is_array()) {
              LOG(WARNING) << "animation action is not array";
              continue;
            }

            std::vector<std::size_t> frames = obj;
            actions.emplace(action, std::move(frames));
          }
        }
      }

      return true;
    }
  }  // namespace R
}  // namespace Exp