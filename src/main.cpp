#include <sol/sol.hpp>

#include <iostream>

int main(int, char*[])
{
  sol::state lua;
  lua.script("foo = 1;");
  std::cout << lua.get<int>("foo") << '\n';
}
