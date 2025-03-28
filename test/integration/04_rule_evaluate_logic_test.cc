#include <gtest/gtest.h>

#include "engine.h"

namespace SrSecurity {
namespace Integration {
TEST(RuleEvaluateLogicTest, evluateLogic) {
  // Test that all variables will be evaluated and the action will be executed every time when the
  // each variable is matched.
  // And any variable is matched, the rule will be matched, and msg and logdata macro will be
  // evaluated.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=3");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo4=bar TX:foo1=bar");
  }

  // Test that chained rule is matched, and starter rule is matched.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        chain, \
        setvar:tx.test=+1"
          SecRule TX:foo1 "@streq bar" "setvar:tx.chain=true")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=3");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo1=bar TX:foo1=bar");
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("chain")), "true");
  }

  // Test that chained rule is not matched, and starter rule is not matched, and the msg and logdata
  // macro will not be evaluated. But the action will be executed.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR}  %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        chain, \
        setvar:tx.test=+1"
          SecRule TX:foo1 "@streq bar12" "setvar:tx.chain=true")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 3);
    EXPECT_FALSE(matched);
    EXPECT_TRUE(t->getMsgMacroExpanded().empty());
    EXPECT_TRUE(t->getLogDataMacroExpanded().empty());
  }
}

TEST(RuleEvaluateLogicTest, exceptVariable) {
  // Test that the except variable is won't be evaluated.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4|!TX:foo1|TX  "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_EQ(std::get<int>(t->getVariable("test")), 4);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=4");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:test=2 TX:foo2=bar123");
  }

  // Test that the except collection is won't be evaluated.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4|!TX "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0, [&](const Rule& rule) { matched = true; });
    EXPECT_FALSE(t->hasVariable("test"));
    EXPECT_FALSE(matched);
  }
}
} // namespace Integration
} // namespace SrSecurity