#include "exp/constants.hpp"
#include "exp/game/camera.hpp"
#include "exp/game/info.hpp"
#include "exp/game/object.hpp"
#include "exp/game/world.hpp"
#include "exp/gl/error_map.hpp"
#include "exp/input/dispatcher.hpp"
#include "exp/input/keyboard.hpp"
#include "exp/io/file.hpp"
#include "exp/render/app_window.hpp"
#include "exp/render/renderer.hpp"
#include "exp/resources/animations.hpp"
#include "exp/resources/game_objects.hpp"
#include "exp/resources/models.hpp"
#include "exp/resources/scripts.hpp"
#include "exp/resources/shaders.hpp"
#include "exp/resources/textures.hpp"
#include "exp/settings/settings_manager.hpp"
#include "exp/ui/ui_manager.hpp"

int main(int, char* argv[])
{
  constexpr const bool PRINT_GL_ERRORS = false;
  using Exp::Game::Camera;
  using Exp::Game::Info;
  using Exp::Game::Object;
  using Exp::Game::World;
  using Exp::Input::Dispatcher;
  using Exp::Input::Keyboard;
  using Exp::IO::File;
  using Exp::R::Animations;
  using Exp::R::GameObjects;
  using Exp::R::Models;
  using Exp::R::Scripts;
  using Exp::R::Shaders;
  using Exp::R::Textures;
  using Exp::Render::AppWindow;
  using Exp::Render::Renderer;
  using Exp::Settings::SettingsManager;
  using Exp::Ui::UiManager;
  using nlohmann::json;

  google::InitGoogleLogging(argv[0]);

  bool exit = false;

  auto& settings = SettingsManager::instance();
  {
    bool loaded = false;
    File::load(Exp::Cfg::File::SETTINGS, [&](const std::string_view src) {
      settings.deserialize(src);
      loaded = true;
    });
    if (!loaded) {
      LOG(FATAL) << "unable to load settings";
    }
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
    shaders.load_all();
  }

  auto& models = Models::instance();
  {
    models.load_all();
  }

  auto& textures = Textures::instance();
  {
    textures.load_all();
  }

  auto& animations = Animations::instance();
  {
    animations.load_all();
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
    window.set_next(&ui);
  }

  auto& camera = Camera::instance();
  {
    float width_2  = settings.window.width / 2.0f;
    float height_2 = settings.window.height / 2.0f;
    camera.set_ortho(-width_2, width_2, -height_2, height_2, settings.game.near_render, settings.game.far_render);
  }

  // rendering
  Renderer renderer{ ui };

  if (!renderer.init()) {
    LOG(ERROR) << "could not init renderer";
    exit = true;
  }

  auto& world = World::instance();

  if (!world.spawn(settings.game.player_object, settings.game.player_location)) {
    LOG(WARNING) << "could not load player object";
  }

  auto& info = Info::instance();

  auto& keyboard = Keyboard::instance();

  Exp::Util::ThreadPool tp(1);

  auto stats_update_timer = std::chrono::system_clock::now();
  const std::chrono::duration<long, std::milli> one_milli(1);
  const std::chrono::duration<long, std::ratio<1>> one_second(1);

  window.show();

  while (!exit) {
    auto start = std::chrono::system_clock::now();

    std::uint16_t fps = settings.game.target_fps;

    auto resume = start + (one_milli * 1000.0 / fps);

    world.finalize_spawns();

    window.poll_events();

    keyboard.update();

    world.update(tp);

    world.render(renderer);

    auto update_check_time = std::chrono::system_clock::now();
    if (update_check_time > stats_update_timer) {
      static std::uint32_t last_count = 0;
      info.fps                        = info.frames - last_count;
      last_count                      = info.frames;
      stats_update_timer              = update_check_time + one_second;

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

  {
    auto player = world.lookup(info.player_id);

    if (player == nullptr) {
      LOG(WARNING) << "player character no longer exists, can't save";
    } else {
      settings.game.player_location = player->location();
    }
  }

  {
    File f;
    f.data = settings.serialize();
    f.save(Exp::Cfg::File::SETTINGS);
  }

  world.release();

  ui.release();

  scripts.release();

  game_objects.release();

  animations.release();

  textures.release();

  models.release();

  shaders.release();

  window.destroy();

  return 0;
}
