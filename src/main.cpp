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
  using nlohmann::json;

  google::InitGoogleLogging(argv[0]);

  DLOG(INFO) << "starting app";

  bool exit = false;

  auto& settings = SettingsManager::instance();
  {
    auto file_res = File::load(ExpGame::SETTINGS_FILE);
    if (!file_res) {
      LOG(FATAL) << "unable to load game settings: " << file_res.err_val();
    }

    auto file = file_res.ok_val();

    settings.deserialize(file.data);
  }

  auto& window = AppWindow::instance();
  {
    window.create();
    window.on_close([&] { exit = true; });
  }

  auto& input = Dispatcher::instance();
  {
    input.set_root_handler(&window);
  }

  auto& shaders = Shaders::instance();
  {
    auto file_res = File::load(ExpGame::SHADER_CFG_FILE);
    if (!file_res) {
      LOG(FATAL) << "unable to load shader configuration file: " << file_res.err_val();
    }

    auto file = file_res.ok_val();

    json shader_json;

    try {
      shader_json = json::parse(file.data);
    } catch (std::exception& e) {
      LOG(FATAL) << "could not parse json: " << e.what();
    }

    shaders.load_all(shader_json);
  }

  auto& ui = UiManager::instance();
  {
    ui.load_all();
  }

  // rendering
  Renderer renderer{ ui };

  const std::chrono::duration<long, std::milli> one_milli(1);
  const std::chrono::duration<long, std::ratio<1>> one_second(1);

  std::uint32_t frame_counter = 0;

  auto stats_update_timer = std::chrono::system_clock::now();

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
