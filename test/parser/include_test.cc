#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
namespace Parser {
class IncludeTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(IncludeTest, Empty) {
  const std::string directive = R"()";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  if (!result.has_value()) {
    std::cout << result.error() << std::endl;
  }

  ASSERT_TRUE(result.has_value());
}

TEST_F(IncludeTest, Comment) {
  const std::string directive = R"(# This is comment1
  # This is comment2
  # This is comment3)";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  if (!result.has_value()) {
    std::cout << result.error() << std::endl;
  }

  ASSERT_TRUE(result.has_value());
}

TEST_F(IncludeTest, Include) {
  const std::string directive = R"(# Test include directive
  Include "test/test_data/include_test.conf"
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  if (!result.has_value()) {
    std::cout << result.error() << std::endl;
  }

  ASSERT_TRUE(result.has_value());
}
} // namespace Parser
} // namespace SrSecurity