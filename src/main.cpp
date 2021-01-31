#include "exp/constants.hpp"
#include "exp/io.hpp"
#include "exp/render.hpp"
#include "exp/settings.hpp"
#include "exp/ui.hpp"
#include "exp/window.hpp"

int main(int, char* argv[])
{
  using ExpGame::Input::Input;
  using ExpGame::IO::File;
  using ExpGame::Render::Renderer;
  using ExpGame::Settings::SettingsManager;
  using ExpGame::Ui::UiManager;
  using ExpGame::Window::Window;

  google::InitGoogleLogging(argv[0]);

  DLOG(INFO) << "Starting app";

  auto file_res = File::load(ExpGame::SETTINGS_FILE);
  if (!file_res) {
    LOG(ERROR) << "unable to load game settings: " << file_res.err_val();
    return 1;
  }

  auto file = file_res.ok_val();

  auto& settings = SettingsManager::instance();
  settings.deserialize(file.data);

  LOG(INFO) << "Creating a window (" << settings.window.width << 'x' << settings.window.height << ")";

  auto& window = Window::instance();
  window.create();

  bool exit = false;

  window.on_close([&] { exit = true; });

  const std::chrono::duration<long, std::milli> one_milli(1);
  const std::chrono::duration<long, std::ratio<1>> one_second(1);

  std::uint32_t frame_counter = 0;

  auto stats_update_timer = std::chrono::system_clock::now();

  LOG(INFO) << "Running main loop with fps target " << settings.game.target_fps;

  auto& input = Input::instance();

  input.set_root_handler(&window);

  auto& ui = UiManager::instance();

  ui.load_all();

  Renderer renderer{ui};

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

  window.destroy();

  return 0;
}
