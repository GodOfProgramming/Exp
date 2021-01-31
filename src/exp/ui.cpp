#include "ui.hpp"

namespace ExpGame
{
  auto UiManager::instance() -> UiManager&
  {
    static UiManager manager;
    return manager;
  }
}  // namespace ExpGame
