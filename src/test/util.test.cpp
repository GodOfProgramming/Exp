#include "exp/util.hpp"

#include <gtest/gtest.h>

using ExpGame::Result;

TEST(Result, static_method_ok_returns_ok_result)
{
  auto res = Result<int>::ok();

  EXPECT_TRUE(!res.is_err());
}

TEST(Result, static_method_err_returns_err_result)
{
  auto res = Result<int>::err(1);

  EXPECT_EQ(res.err(), 1);
}

TEST(Result, calling_err_on_ok_returns_default_value)
{
  auto res = Result<int>::ok();

  EXPECT_EQ(res.err(), 0);
}
