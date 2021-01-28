#include <GLFW/glfw3.h>
#include <sol/sol.hpp>

#include <iostream>

int main(int, char*[])
{
  sol::state state;
  state.script("foo = 1;");
  std::cout << state.get<int>("foo") << '\n';

  return 0;
}
