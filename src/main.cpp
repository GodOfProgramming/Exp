#include "exp/constants.hpp"
#include "exp/game/info.hpp"
#include "exp/game/object.hpp"
#include "exp/gl/error_map.hpp"
#include "exp/input/dispatcher.hpp"
#include "exp/io/file.hpp"
#include "exp/render/app_window.hpp"
#include "exp/render/renderer.hpp"
#include "exp/resources/game_objects.hpp"
#include "exp/resources/models.hpp"
#include "exp/resources/scripts.hpp"
#include "exp/resources/shaders.hpp"
#include "exp/settings/settings_manager.hpp"
#include "exp/ui/ui_manager.hpp"

int main(int, char* argv[])
{
  constexpr const bool PRINT_GL_ERRORS = false;
  using Exp::Game::Info;
  using Exp::Game::Object;
  using Exp::Input::Dispatcher;
  using Exp::IO::File;
  using Exp::Render::AppWindow;
  using Exp::Render::Renderer;
  using Exp::Resources::GameObjects;
  using Exp::Resources::Models;
  using Exp::Resources::Scripts;
  using Exp::Resources::Shaders;
  using Exp::Settings::SettingsManager;
  using Exp::Ui::UiManager;
  using nlohmann::json;

  google::InitGoogleLogging(argv[0]);

  bool exit = false;

  auto& settings = SettingsManager::instance();
  {
    auto file_res = File::load(Exp::SETTINGS_FILE);
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
    auto file_res = File::load(Exp::SHADER_CFG_FILE);
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

  auto& models = Models::instance();
  {
    models.load_all();
  }

  auto& game_objects = GameObjects::instance();
  {
    game_objects.load_all();
  }

  auto& scripts = Scripts::instance();
  {
    scripts.load_all();
  }

  auto& ui = UiManager::instance();
  {
    ui.load_all();
  }

  // rendering
  Renderer renderer{ ui };

  const std::chrono::duration<long, std::milli> one_milli(1);
  const std::chrono::duration<long, std::ratio<1>> one_second(1);

  auto stats_update_timer = std::chrono::system_clock::now();

  std::vector<std::shared_ptr<Object>> objects;
  auto obj = game_objects.find("exp.debug.random.square");
  if (obj == game_objects.end()) {
    LOG(FATAL) << "could not even load the friggen debug object, nice job dumbass";
  }
  objects.push_back(std::make_shared<Object>(obj->second));

  auto& info = Info::instance();

  window.show();

  while (!exit) {
    std::uint16_t fps = settings.game.target_fps;

    auto start = std::chrono::system_clock::now();

    auto resume = start + (one_milli * 1000.0 / fps);

    window.poll_events();

    renderer.render_to(window, objects);

    auto update_check_time = std::chrono::system_clock::now();
    if (update_check_time > stats_update_timer) {
      static std::uint32_t last_count = 0;
      info.fps                        = info.frames - last_count;
      LOG(INFO) << "FPS: " << info.fps;
      last_count         = info.frames;
      stats_update_timer = update_check_time + one_second;

      if constexpr (PRINT_GL_ERRORS) {
        auto& errors = Exp::GL::ErrorMap::instance();
        for (const auto& error : errors) {
          LOG(INFO) << "Error " << error.first << ": " << error.second.desc;
          for (const auto& occurrance : error.second.occurrences) {
            LOG(INFO) << "\tFile: " << occurrance.first;
            for (const auto line : occurrance.second) { LOG(INFO) << "\tLine" << line.first << " - " << line.second; }
          }
        }
      }
    }

    info.frames++;

    std::this_thread::sleep_until(resume);
  }

  objects.clear();

  ui.shutdown();

  scripts.release();

  game_objects.release();

  models.release();

  shaders.release();

  window.destroy();

  return 0;
}
