#pragma once

#include "setting.hpp"

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
        Setting<std::string> title;
        Setting<int> height;
        Setting<int> width;
      } window;

      struct
      {
        Setting<std::uint16_t> target_fps;
      } game;

     private:
    };
  }  // namespace Settings
}  // namespace Exp