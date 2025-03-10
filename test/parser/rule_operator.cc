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
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar"
      SecRule TX:foo "@beginsWith ba" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
      SecRule TX:foo "@beginsWith ar" "id:1,phase:2,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, beginsWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar,setvar:tx.bar=bar,setvar:tx.bar1=bar1"
  SecRule TX:foo "@beginsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
  SecRule TX:foo "@beginsWith %{tx.bar1}" "id:2,phase:1,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, ipMatch) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.ipv4=192.168.1.1"
      SecAction "phase:1,setvar:tx.ipv6=2001:db8:85a3:8d3:1319:8a2e:370:7348"
  SecRule TX:ipv4 "@ipMatch 192.168.1.1" "id:1,phase:1,setvar:'tx.ipv4_true'"
  SecRule TX:ipv4 "@ipMatch 192.168.1.2" "id:1,phase:1,setvar:'tx.ipv4_false'"
  SecRule TX:ipv4 "@ipMatch 192.168.1.0/24" "id:2,phase:1,setvar:'tx.ipv4_mark_true'"
  SecRule TX:ipv4 "@ipMatch 192.168.100.0/24" "id:2,phase:1,setvar:'tx.ipv4_mark_false'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:370:7348" "id:1,phase:1,setvar:'tx.ipv6_true'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:370:7349" "id:1,phase:1,setvar:'tx.ipv6_false'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:370:0000/24" "id:1,phase:1,setvar:'tx.ipv6_mask_true'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:270:0000/24" "id:1,phase:1,setvar:'tx.ipv6_mask_false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("ipv4_true"));
  EXPECT_FALSE(t->hasVariable("ipv4_false"));
  EXPECT_TRUE(t->hasVariable("ipv4_mark_true"));
  EXPECT_FALSE(t->hasVariable("ipv4_mark_false"));
  EXPECT_TRUE(t->hasVariable("ipv6_true"));
  EXPECT_FALSE(t->hasVariable("ipv6_false"));
  EXPECT_TRUE(t->hasVariable("ipv6_mask_true"));
  EXPECT_FALSE(t->hasVariable("ipv6_mask_false"));
}

TEST_F(RuleOperatorTest, within) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld"
      SecRule TX:foo "@within hello" "id:1,phase:1,setvar:'tx.true1'"
      SecRule TX:foo "@within hello " "id:2,phase:1,setvar:'tx.true2'"
      SecRule TX:foo "@within hello1 world" "id:3,phase:1,setvar:'tx.true3'"
      SecRule TX:foo "@within hello1" "id:4,phase:1,setvar:'tx.false1'"
      SecRule TX:foo "@within hello1 world1" "id:5,phase:1,setvar:'tx.false2'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_TRUE(t->hasVariable("true3"));
  EXPECT_FALSE(t->hasVariable("false1"));
  EXPECT_FALSE(t->hasVariable("false2"));
}

TEST_F(RuleOperatorTest, withinWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld"
      SecAction "phase:1,setvar:tx.v1=hello"
      SecAction "phase:1,setvar:tx.v2=hello "
      SecAction "phase:1,setvar:tx.v3=hello1 world"
      SecAction "phase:1,setvar:tx.v4=hello1"
      SecAction "phase:1,setvar:tx.v5=hello1 world1"
      SecRule TX:foo "@within %{tx.v1}" "id:1,phase:1,setvar:'tx.true1'"
      SecRule TX:foo "@within %{tx.v2}" "id:2,phase:1,setvar:'tx.true2'"
      SecRule TX:foo "@within %{tx.v3}" "id:3,phase:1,setvar:'tx.true3'"
      SecRule TX:foo "@within %{tx.v4} %{tx.v1}" "id:3,phase:1,setvar:'tx.true4'"
      SecRule TX:foo "@within %{tx.v4}" "id:4,phase:1,setvar:'tx.false1'"
      SecRule TX:foo "@within %{tx.v5}" "id:5,phase:1,setvar:'tx.false2'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_TRUE(t->hasVariable("true3"));
  EXPECT_TRUE(t->hasVariable("true4"));
  EXPECT_FALSE(t->hasVariable("false1"));
  EXPECT_FALSE(t->hasVariable("false2"));
}
} // namespace Parser
} // namespace SrSecurity