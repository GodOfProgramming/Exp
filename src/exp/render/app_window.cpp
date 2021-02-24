#include "app_window.hpp"

#include "exp/game/camera.hpp"
#include "exp/game/info.hpp"
#include "exp/gl/error_map.hpp"
#include "exp/input/dispatcher.hpp"
#include "exp/input/key_event.hpp"
#include "exp/input/mouse_button_event.hpp"
#include "exp/input/mouse_move_event.hpp"
#include "exp/settings/settings_manager.hpp"

namespace Exp
{
  namespace Render
  {
    using Input::KeyEvent;
    using Input::MouseButtonEvent;
    using Input::MouseMoveEvent;
    using Settings::SettingsManager;

    auto AppWindow::instance() -> AppWindow&
    {
      static AppWindow window;
      return window;
    }

    AppWindow::AppWindow()
    {
      if (glfwInit() != GLFW_TRUE) {
        LOG(FATAL) << "could not initialize glfw";
      }
    }

    void AppWindow::create()
    {
      LOG(INFO) << "creating window";
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
      glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
      glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

      auto& settings = SettingsManager::instance();

      this->window = glfwCreateWindow(settings.window.width, settings.window.height, settings.window.title.raw().c_str(), nullptr, nullptr);
      if (window == nullptr) {
        LOG(FATAL) << "failed to create glfw window";
      }

      glfwMakeContextCurrent(this->window);

      if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        LOG(FATAL) << "failed to initialize GLAD";
      }

      glViewport(0, 0, settings.window.width, settings.window.height);

      settings.window.width.on_change([this](auto, auto width) {
        auto& settings = SettingsManager::instance();
        glfwSetWindowSize(this->window, width, settings.window.height);
      });

      settings.window.height.on_change([this](auto, auto height) {
        auto& settings = SettingsManager::instance();
        glfwSetWindowSize(this->window, settings.window.width, height);
      });

      glfwSetFramebufferSizeCallback(this->window, [](GLFWwindow*, int width, int height) {
        auto& settings         = SettingsManager::instance();
        settings.window.width  = width;
        settings.window.height = height;
        auto& camera           = Game::Camera::instance();
        camera.set_ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height), settings.game.near_render, settings.game.far_render);
        glViewport(0, 0, settings.window.width, settings.window.height);
      });

      glfwSetWindowCloseCallback(this->window, [](GLFWwindow*) {
        auto& window = AppWindow::instance();
        window.close();
      });

      glfwSetKeyCallback(this->window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        auto& input = Input::Dispatcher::instance();

        Input::KeyEvent event;
        event.key    = static_cast<Input::Key>(key);
        event.action = static_cast<Input::Action>(action);

        input.process(event);

        ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
      });

      glfwSetScrollCallback(this->window, [](GLFWwindow* window, double x, double y) { ImGui_ImplGlfw_ScrollCallback(window, x, y); });

      glfwSetMouseButtonCallback(
       this->window, [](GLFWwindow* window, int button, int action, int mods) { ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods); });

      glfwSetCharCallback(this->window, [](GLFWwindow* window, unsigned int c) { ImGui_ImplGlfw_CharCallback(window, c); });

      LOG(INFO) << "GLFW Version: " << glfwGetVersionString();
      LOG(INFO) << "OpenGL Renderer: " << glGetString(GL_RENDERER);
      GL_CHECK();
      LOG(INFO) << "OpenGL Version: " << glGetString(GL_VERSION);
      GL_CHECK();
    }

    void AppWindow::show()
    {
      glfwShowWindow(this->window);
    }

    void AppWindow::hide()
    {
      glfwHideWindow(this->window);
    }

    void AppWindow::swap_buffers()
    {
      glfwSwapBuffers(this->window);
    }

    void AppWindow::poll_events()
    {
      glfwPollEvents();
    }

    void AppWindow::close()
    {
      this->on_close_callback();
      glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }

    void AppWindow::destroy()
    {
      glfwDestroyWindow(this->window);
      glfwTerminate();
      this->window = nullptr;
    }

    void AppWindow::on_close(std::function<void(void)> f)
    {
      this->on_close_callback = f;
    }

    auto AppWindow::get_size() -> glm::ivec2
    {
      glm::ivec2 dim;
      glfwGetWindowSize(this->window, &dim.x, &dim.y);
      return dim;
    }

    auto AppWindow::handle(Input::KeyEvent e) -> Input::IHandler*
    {
      switch (e.key) {
        case Input::Key::ESC: {
          if (e.action == Input::Action::PRESS) {
            auto& info = Game::Info::instance();
            info.state = info.state == Game::State::PAUSED ? Game::State::PLAYING : Game::State::PAUSED;
          }
        } break;
        default: {
          return this->get_next();
        }
      }

      return nullptr;
    }

    auto AppWindow::handle(Input::MouseButtonEvent) -> Input::IHandler*
    {
      return this->get_next();
    }

    auto AppWindow::handle(Input::MouseMoveEvent) -> Input::IHandler*
    {
      return this->get_next();
    }

    auto AppWindow::operator*() -> WindowHandle
    {
      return this->window;
    }
  }  // namespace Render
}  // namespace Exp
