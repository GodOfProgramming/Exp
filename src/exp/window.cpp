#include "window.hpp"

#include "settings.hpp"

namespace ExpGame
{
  auto Window::instance() -> Window&
  {
    static Window window;
    return window;
  }

  Window::Window()
  {
    if (glfwInit() != GLFW_TRUE) {
      LOG(FATAL) << "Could not initialize glfw";
    }
  }

  Window::~Window() {}

  void Window::create()
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

    glfwSetWindowCloseCallback(this->window, [](auto) {
      auto& window = Window::instance();
      window.on_close_callback();
    });
  }

  void Window::swap_buffers()
  {
    glfwSwapBuffers(this->window);
  }

  void Window::poll_events()
  {
    glfwPollEvents();
  }

  void Window::on_close(std::function<void(void)> f)
  {
    this->on_close_callback = f;
  }
}  // namespace ExpGame
