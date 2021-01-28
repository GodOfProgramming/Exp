#include "exp/settings.hpp"
#include "exp/window.hpp"

int main(int, char*[])
{
  ExpGame::SettingsManager settings;

  settings.load(R"({ "name": "value" })");
}
