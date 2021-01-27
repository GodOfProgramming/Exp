#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
namespace
{
  constexpr bool REROUTE_STDOUT = false;
}

int main(int argc, char* argv[])
{
  std::streambuf* old = std::cout.rdbuf();
  std::ofstream null("/dev/null");

  if (REROUTE_STDOUT) {
    std::cout.rdbuf(null.rdbuf());
  }

  testing::InitGoogleTest(&argc, argv);

  int res = RUN_ALL_TESTS();

  if (REROUTE_STDOUT) {
    std::cout.rdbuf(old);
  }

  return res;
}
