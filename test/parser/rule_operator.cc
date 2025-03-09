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

public:
  Engine engine_;
};

TEST_F(RuleOperatorTest, OperatorBeginWith) {
  const std::string rule_directive =
      R"(SecRule ARGS "@beginsWith hello" "id:1,tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(rule_directive);
  ASSERT_TRUE(result.has_value());

  auto& op = parser.rules().back()->getOperator();
  EXPECT_EQ(op->name(), std::string("beginsWith"));
  EXPECT_EQ(op->literalValue(), "hello");

  // Macro expansion
  {
    const std::string rule_directive =
        R"(SecAction "phase:1,setvar:tx.foo=bar"
        SecRule ARGS "@beginsWith %{tx.foo}" "id:1,phase:1,tag:'foo',msg:'bar'")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(rule_directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    engine.rules(1).front()->actions().front()->evaluate(*t);
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("foo")), "bar");

    auto& op = engine.rules(1).back()->getOperator();
    EXPECT_EQ(op->name(), std::string("beginsWith"));
    EXPECT_TRUE(op->literalValue().empty());
    auto macro = op->macro();
    ASSERT_NE(macro, nullptr);
    EXPECT_EQ(std::get<std::string_view>(macro->evaluate(*t)), "bar");
  }
}

} // namespace Parser
} // namespace SrSecurity