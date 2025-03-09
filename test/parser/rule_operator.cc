#include <format>
#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <stdlib.h>

#include "action/actions_include.h"
#include "antlr4/parser.h"
#include "engine.h"
#include "transformation/transform_include.h"
#include "variable/variables_include.h"

namespace SrSecurity {
namespace Parser {
class RuleOperatorTest : public testing::Test {
public:
  RuleOperatorTest() : engine_(spdlog::level::trace) {}

  void SetUp() override {
    std::string directiv = R"(SecRuleEngine On)";
    engine_.load(directiv);
  }

public:
  Engine engine_;
};

TEST_F(RuleOperatorTest, beginsWith) {
  const std::string rule_directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar"
      SecRule TX:foo "@beginsWith ba" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
      SecRule TX:foo "@beginsWith ar" "id:1,phase:2,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(rule_directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, beginsWithMacro) {
  const std::string rule_directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar,setvar:tx.bar=bar,setvar:tx.bar1=bar1"
  SecRule TX:foo "@beginsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
  SecRule TX:foo "@beginsWith %{tx.bar1}" "id:2,phase:1,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(rule_directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}
} // namespace Parser
} // namespace SrSecurity