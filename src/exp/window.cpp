#include "window.hpp"

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
      throw std::runtime_error("could not initialize glfw");
    }
  }

  Window::~Window() {}

  auto Window::create() -> CreateResult
  {
    return CreateResult::ok();
  }
}  // namespace ExpGame
