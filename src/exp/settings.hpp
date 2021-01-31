#pragma once

#include "util.hpp"

namespace ExpGame
{
  namespace Settings
  {
    template <typename T>
    class Setting
    {
      using ChangeCallback = std::function<void(Setting&, T)>;

     public:
      using type = T;

      Setting() = default;
      Setting(std::string n, T v)
       : name(n)
       , value(v)
      {}

      auto display_name() const noexcept -> T
      {
        return this->name;
      }

      auto raw() const noexcept -> T
      {
        return this->value;
      }

      operator T()
      {
        return this->value;
      }

      /**
       * @brief First calls all callbacks with the current value of the setting and the to soon be.
       */
      void update(T& v)
      {
        for (const auto& callback : this->callbacks) { callback(*this, v); }
        this->value = v;
      }

      auto operator=(T& v) -> Setting&
      {
        this->value = v;
        return *this;
      }

      auto on_change(ChangeCallback f)
      {
        this->callbacks.push_back(f);
      }

     private:
      std::string name;
      T value;
      std::vector<ChangeCallback> callbacks;
    };

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
}  // namespace ExpGame
