#pragma once

namespace Exp
{
  namespace Settings
  {
    class SettingsManager
    {
      SettingsManager() = default;

     public:
      SettingsManager(const SettingsManager&) = delete;
      SettingsManager(SettingsManager&&)      = delete;
      auto operator=(const SettingsManager&) -> SettingsManager& = delete;
      auto operator=(SettingsManager&&) -> SettingsManager& = delete;

      ~SettingsManager();

      static auto instance() -> SettingsManager&;

      auto serialize() -> std::string;

      /**
       * @brief Deserializes the provided string. If the string is not valid json, the program terminates
       */
      void deserialize(std::string_view raw);

      struct
      {
        std::string title;
        int height;
        int width;
      } window;

      struct
      {
        std::uint16_t target_fps;
        float near_render;
        float far_render;
        std::string player_object;
        glm::vec3 player_location;
      } game;
    };
  }  // namespace Settings
}  // namespace Exp