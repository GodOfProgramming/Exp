#include "exp/file.hpp"
#include "exp/settings.hpp"
#include "exp/window.hpp"

int main(int, char*[])
{
  auto file_res = ExpGame::File::load("assets/settings.json");
  if (!file_res) {
    std::cout << "unable to load game settings: " << file_res.err_val() << '\n';
    return 1;
  }

  auto file = file_res.ok_val();

  ExpGame::SettingsManager settings;

  auto settings_res = settings.load(file.data);

  if (!settings_res) {
    std::cout << "unable to parse game settings: " << settings_res.err_val() << '\n';
    return 1;
  }
}
