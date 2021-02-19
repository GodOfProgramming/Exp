#include "exp/util.hpp"

using Exp::Util::Result;

using TestResult = Result<std::string, std::string>;

TEST(Result, static_method_ok_returns_ok_result)
{
  auto res1 = TestResult::ok();
  auto res2 = TestResult::ok("1");

  EXPECT_TRUE(res1.is_ok());
  EXPECT_TRUE(res2.is_ok());

  EXPECT_FALSE(res1.is_err());
  EXPECT_FALSE(res2.is_err());
}

TEST(Result, static_method_err_returns_err_result)
{
  auto res = TestResult::err("1");

  EXPECT_FALSE(res.is_ok());
  EXPECT_TRUE(res.is_err());
  EXPECT_EQ(res.err_val(), "1");
}

TEST(Result, getting_ok_or_err_value_on_empty_result_returns_default_values)
{
  auto ok  = TestResult::ok();
  auto err = TestResult::err("1");

  EXPECT_EQ(ok.err_val(), std::string());
  EXPECT_EQ(err.ok_val(), std::string());
}
