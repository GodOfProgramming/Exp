#pragma once

#include "exp/util/exceptions.hpp"

namespace Exp
{
  namespace Settings
  {
    class SettingsManager
    {
      using DoNotUseException = Util::DoNotUseException;

     public:
      SettingsManager() noexcept = default;
      SettingsManager(const SettingsManager&);
      SettingsManager(SettingsManager&&) noexcept = default;
      auto operator                               =(const SettingsManager&) -> SettingsManager&;
      auto operator=(SettingsManager&&) noexcept -> SettingsManager& = default;

      ~SettingsManager();

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
        std::size_t thread_count;
      } game;
    };
  }  // namespace Settings
}  // namespace Exp