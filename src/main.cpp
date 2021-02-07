#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/render.hpp"
#include "exp/resources.hpp"
#include "exp/settings.hpp"
#include "exp/ui.hpp"
#include "exp/window.hpp"

int main(int, char* argv[])
{
  using ExpGame::Input::Dispatcher;
  using ExpGame::IO::File;
  using ExpGame::Render::Renderer;
  using ExpGame::Resources::Shaders;
  using ExpGame::Settings::SettingsManager;
  using ExpGame::Ui::UiManager;
  using ExpGame::Window::AppWindow;

  google::InitGoogleLogging(argv[0]);

  DLOG(INFO) << "starting app";

  auto settings_file_res = File::load(ExpGame::SETTINGS_FILE);
  if (!settings_file_res) {
    LOG(ERROR) << "unable to load game settings: " << settings_file_res.err_val();
    return 1;
  }

  auto settings_file = settings_file_res.ok_val();

  auto& settings = SettingsManager::instance();
  settings.deserialize(settings_file.data);

  auto& window = AppWindow::instance();
  window.create();

  bool exit = false;

  window.on_close([&] { exit = true; });

  std::uint32_t frame_counter = 0;

  auto stats_update_timer = std::chrono::system_clock::now();

  auto& input = Dispatcher::instance();
  input.set_root_handler(&window);

  auto& shaders = Shaders::instance();
  shaders.load_all();

  auto& ui = UiManager::instance();
  ui.load_all();

  Renderer renderer{ ui };

  const std::chrono::duration<long, std::milli> one_milli(1);
  const std::chrono::duration<long, std::ratio<1>> one_second(1);

  window.show();

  while (!exit) {
    std::uint16_t fps = settings.game.target_fps;

    auto start = std::chrono::system_clock::now();

    auto resume = start + (one_milli * 1000.0 / fps);

    window.poll_events();

    renderer.render_to(window);

    auto update_check_time = std::chrono::system_clock::now();
    if (update_check_time > stats_update_timer) {
      static std::uint32_t last_count = 0;
      LOG(INFO) << "FPS: " << frame_counter - last_count;
      last_count         = frame_counter;
      stats_update_timer = update_check_time + one_second;
    }

    frame_counter++;

    std::this_thread::sleep_until(resume);
  }

  ui.shutdown();

  shaders.release();

  window.destroy();

  return 0;
}
