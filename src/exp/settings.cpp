#include "settings.hpp"

namespace ExpGame
{
  using nlohmann::json;

  auto SettingsManager::instance() -> SettingsManager&
  {
    static SettingsManager manager;
    return manager;
  }

  SettingsManager::SettingsManager() {}
  SettingsManager::~SettingsManager() {}

  auto SettingsManager::load(std::string_view raw) -> LoadResult
  {
    try {
      json j = json::parse(std::string(raw));
    } catch (std::exception& e) {
      std::stringstream ss;
      ss << "could not parse json: " << e.what();
      return LoadResult::err(ss.str());
    }

    return LoadResult::ok();
  }
}  // namespace ExpGame
