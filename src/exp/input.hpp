#pragma once

namespace ExpGame
{
  namespace Input
  {
    enum class Key
    {
      ESC = GLFW_KEY_ESCAPE,
    };

    enum class Action
    {
      Press   = GLFW_PRESS,
      Release = GLFW_RELEASE,
    };

    struct KeyEvent
    {
      Key key;
      Action action;
    };

    struct MouseButtonEvent
    {};

    struct MouseMoveEvent
    {};

    class IHandler
    {
     public:
      IHandler();
      virtual ~IHandler() = default;

      virtual auto handle(KeyEvent) -> IHandler*         = 0;
      virtual auto handle(MouseButtonEvent) -> IHandler* = 0;
      virtual auto handle(MouseMoveEvent) -> IHandler*   = 0;

      void set_next(IHandler* next);
      auto get_next() -> IHandler*;

     private:
      IHandler* next;
    };

    class Dispatcher
    {
      Dispatcher();

     public:
      Dispatcher(const Dispatcher&) = delete;
      Dispatcher(Dispatcher&&)      = delete;
      auto operator=(const Dispatcher&) -> Dispatcher& = delete;
      auto operator=(Dispatcher&&) -> Dispatcher& = delete;

      static auto instance() -> Dispatcher&;

      void set_root_handler(IHandler* handler);

      void process(KeyEvent event);
      void process(MouseButtonEvent event);
      void process(MouseMoveEvent event);

     private:
      IHandler* root_handler;
    };
  }  // namespace Input
}  // namespace ExpGame
