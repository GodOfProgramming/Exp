#include "window.hpp"

#include "settings.hpp"

namespace ExpGame
{
  namespace Window
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
        LOG(FATAL) << "Could not initialize glfw";
      }
    }

    void AppWindow::create()
    {
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

      auto& settings = SettingsManager::instance();

      this->window =
       glfwCreateWindow(settings.window.width, settings.window.height, settings.window.title.raw().c_str(), nullptr, nullptr);
      if (window == nullptr) {
        LOG(FATAL) << "Failed to create glfw window";
      }

      glfwMakeContextCurrent(this->window);

      if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        LOG(FATAL) << "Failed to initialize GLAD";
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

      glfwSetScrollCallback(
       this->window, [](GLFWwindow* window, double x, double y) { ImGui_ImplGlfw_ScrollCallback(window, x, y); });

      glfwSetMouseButtonCallback(this->window, [](GLFWwindow* window, int button, int action, int mods) {
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
      });

      glfwSetCharCallback(this->window, [](GLFWwindow* window, unsigned int c) { ImGui_ImplGlfw_CharCallback(window, c); });
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
      DLOG(INFO) << "Closing window";
      this->on_close_callback();
      glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }

    void AppWindow::destroy()
    {
      DLOG(INFO) << "Destorying window";
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
          if (e.action == Input::Action::Press) {
            this->close();
          }
        } break;
        default: {
          return this->get_next();
        }
      }

      return nullptr;
    }

    auto AppWindow::handle(Input::MouseButtonEvent e) -> Input::IHandler*
    {
      return this->get_next();
    }

    auto AppWindow::handle(Input::MouseMoveEvent e) -> Input::IHandler*
    {
      return this->get_next();
    }

    auto AppWindow::operator*() -> WindowHandle
    {
      return this->window;
    }
  }  // namespace Window
}  // namespace ExpGame
