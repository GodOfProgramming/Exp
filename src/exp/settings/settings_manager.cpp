#include "settings_manager.hpp"

#define ASSIGN(obj, field, default_value)                 \
  do {                                                    \
    using setting_t = decltype(this->obj.field);          \
    using field_t   = setting_t::type;                    \
    auto f          = obj[#field];                        \
    if (f.is_null()) {                                    \
      this->obj.field = setting_t(#field, default_value); \
    } else {                                              \
      field_t field   = f;                                \
      this->obj.field = field;                            \
    }                                                     \
  } while (false)

namespace Exp
{
  namespace Settings
  {
    using nlohmann::json;

    auto SettingsManager::instance() -> SettingsManager&
    {
      static SettingsManager manager;
      return manager;
    }

    SettingsManager::~SettingsManager() {}

    auto SettingsManager::serialize() -> std::string
    {
      json j = { { "window",
                   { { "title", this->window.title.raw() }, { "height", this->window.height.raw() }, { "width", this->window.width.raw() } },
                   { "game", { "target_fps", 60 } } } };

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
      ASSIGN(window, title, "ExpGame");
      ASSIGN(window, height, 720);
      ASSIGN(window, width, 1280);

      auto game = j["game"];
      ASSIGN(game, target_fps, 60);
    }
  }  // namespace Settings
}  // namespace Exp