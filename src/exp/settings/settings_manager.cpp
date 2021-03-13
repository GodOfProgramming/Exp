#include "settings_manager.hpp"

namespace Exp
{
  namespace Settings
  {
    using nlohmann::json;

    template <typename T, typename D = T>
    void default_if_null(T& value, D default_value, json& j, std::string field, std::function<T(json&)> f)
    {
      json& json_field = j[field];
      if (json_field.is_null()) {
        value = default_value;
      } else {
        value = f(json_field);
      }
    }

    auto SettingsManager::instance() -> SettingsManager&
    {
      static SettingsManager manager;
      return manager;
    }

    SettingsManager::~SettingsManager() {}

    auto SettingsManager::serialize() -> std::string
    {
      json j = {
        {
         "window",
         {
          { "title", this->window.title },
          { "height", this->window.height },
          { "width", this->window.width },
         },
        },
        {
         "game",
         {
          { "target_fps", this->game.target_fps },
          { "near_render", this->game.near_render },
          { "far_render", this->game.far_render },
          { "player_object", this->game.player_object },
          { "player_location", { this->game.player_location.x, this->game.player_location.y, this->game.player_location.z } },
          { "thread_count", this->game.thread_count },
         },
        },
      };

      return j.dump(2);
    }

    void SettingsManager::deserialize(std::string_view raw)
    {
      json j;

      try {
        j = json::parse(std::string(raw));
      } catch (std::exception& e) {
        LOG(FATAL) << "could not parse json: " << e.what();
      }

      auto window = j["window"];
      default_if_null<std::string, const char*>(this->window.title, "ExpGame", window, "title", [](json& j) { return j; });
      default_if_null<int>(this->window.width, 1280, window, "width", [](json& j) { return j; });
      default_if_null<int>(this->window.height, 720, window, "height", [](json& j) { return j; });

      auto game = j["game"];
      default_if_null<std::uint16_t>(this->game.target_fps, 60, game, "target_fps", [](json& j) { return j; });
      default_if_null<float>(this->game.near_render, 0.0, game, "near_render", [](json& j) { return j; });
      default_if_null<float>(this->game.far_render, 100.0, game, "far_render", [](json& j) { return j; });
      default_if_null<std::string, const char*>(this->game.player_object, "exp.characters.main.player", game, "player_object", [](json& j) { return j; });
      default_if_null<glm::vec3>(this->game.player_location, glm::vec3{ 0, 0, 0 }, game, "player_location", [](json& j) {
        glm::vec3 ret{ 0, 0, 0 };
        if (j.is_array() && j.size() == 3) {
          ret[0] = j[0];
          ret[1] = j[1];
          ret[2] = j[2];
        }
        return ret;
      });
      default_if_null<std::size_t>(this->game.thread_count, 3, game, "thread_count", [](json& j) { return j; });
    }
  }  // namespace Settings
}  // namespace Exp