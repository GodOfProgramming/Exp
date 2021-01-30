#include "exp/file.hpp"
#include "exp/settings.hpp"
#include "exp/window.hpp"

int main(int, char* argv[])
{
  google::InitGoogleLogging(argv[0]);

  DLOG(INFO) << "starting app";
  auto file_res = ExpGame::File::load("assets/settings.json");
  if (!file_res) {
    std::cout << "unable to load game settings: " << file_res.err_val() << '\n';
    return 1;
  }

  auto file = file_res.ok_val();

  auto& settings = ExpGame::SettingsManager::instance();

  auto settings_res = settings.load(file.data);

  if (!settings_res) {
    std::cout << ExpGame::to_string("unable to parse game settings: ", settings_res.err_val(), '\n');
    return 1;
  }

  auto& window = ExpGame::Window::instance();

  window.create();

  return 0;
}
