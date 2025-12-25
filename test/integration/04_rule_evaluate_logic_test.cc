/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <gtest/gtest.h>

#include "engine.h"

namespace Wge {
namespace Integration {
TEST(RuleEvaluateLogicTest, evluateLogic) {
  // Test that all variables will be evaluated and the action will be executed every time when the
  // each variable is matched.
  // And any variable is matched, the rule will be matched, and msg and logdata macro will be
  // evaluated.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=%42ar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4 "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        log, \
        t:none, \
        t:urlDecode, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VAR_NAME}=%{MATCHED_VAR} %{MATCHED_VARS_NAMES}=%{MATCHED_VARS}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 3);
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
        log, \
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
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 3);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=3");
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo4=bar TX:foo1=bar");
    EXPECT_EQ(std::get<std::string_view>(t->getVariable("", "chain")), "true");
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
        log, \
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
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 3);
    EXPECT_FALSE(matched);

    // Even though the rule is not matched, the msg and logdata macro can evluate manually.
    EXPECT_FALSE(t->getMsgMacroExpanded().empty());
    EXPECT_FALSE(t->getLogDataMacroExpanded().empty());
  }
}

TEST(RuleEvaluateLogicTest, cartesianProduct) {
  const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=foo,setvar:tx.bar=bar,setvar:tx.baz=baz,setvar:tx.matched_count=0,setvar:tx.unmatched_count=0,setvar:tx.total_count=0"
        SecRule TX "@streq %{TX}"  "phase:1,id:1, *setvar:tx.total_count=+1, setvar:tx.matched_count=+1, !setvar:tx.unmatched_count=+1"
    )";

  Engine engine(spdlog::level::off);
  auto result = engine.load(directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  bool matched = false;
  t->processRequestHeaders(nullptr, nullptr, 0);
  EXPECT_EQ(std::get<int64_t>(t->getVariable("", "total_count")), 6 * 6);
}

TEST(RuleEvaluateLogicTest, operatorOrCombination) {
  const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=foo"
        SecRule TX:foo "@streq hello|@beginsWith world|@streq foo|@rx hi"  "phase:1,id:1, setvar:tx.matched"
    )";

  Engine engine(spdlog::level::off);
  auto result = engine.load(directive);
  engine.init();
  auto t = engine.makeTransaction();
  ASSERT_TRUE(result.has_value());

  bool matched = false;
  t->processRequestHeaders(nullptr, nullptr, 0);
  EXPECT_TRUE(t->hasVariable("", "matched"));
}

TEST(RuleEvaluateLogicTest, unmatchedBranch) {
  // Test the ALWAYS and UNMATCHED action branches.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=foo"
        SecRule TX:foo "@streq foo"  "phase:1,id:1, !setvar:tx.unmatched0, setvar:tx.matched0, *setvar:tx.always0"
        SecRule TX:foo "!@streq foo"  "phase:1,id:2, !setvar:tx.unmatched1, setvar:tx.matched1,*setvar:tx.always1"
    )";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0);
    EXPECT_TRUE(t->hasVariable("", "matched0"));
    EXPECT_FALSE(t->hasVariable("", "unmatched0"));
    EXPECT_TRUE(t->hasVariable("", "always0"));
    EXPECT_FALSE(t->hasVariable("", "matched1"));
    EXPECT_TRUE(t->hasVariable("", "unmatched1"));
    EXPECT_TRUE(t->hasVariable("", "always1"));
  }

  // Test the chained rules with ALWAYS and UNMATCHED action branches.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=foo"
        SecRule TX:foo "!@streq foo"  "phase:1,id:1, chain"
          SecRule TX:foo "@streq foo"  "setvar:tx.v1"
        SecRule TX:foo "!@streq foo"  "phase:1,id:2, !chain"
          SecRule TX:foo "@streq foo"  "setvar:tx.v2"
        SecRule TX:foo "!@streq foo"  "phase:1,id:3, *chain"
          SecRule TX:foo "@streq foo"  "setvar:tx.v3"
    )";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0);
    EXPECT_FALSE(t->hasVariable("", "v1"));
    EXPECT_TRUE(t->hasVariable("", "v2"));
    EXPECT_TRUE(t->hasVariable("", "v3"));
  }
}

TEST(RuleEvaluateLogicTest, multiChain) {
  // Test the multiChain for matched branch
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=100,setvar:tx.bar=200,setvar:tx.baz=300"
        SecRule TX "@lt 300"  "phase:1,id:1, multiChain"
          SecRule TX:foo "@unconditionalMatch"  "setvar:tx.test=+1"
    )";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 2);
  }

  // Test the multiChain for unmatched branch
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo=100,setvar:tx.bar=200,setvar:tx.baz=300"
        SecRule TX "@lt 300"  "phase:1,id:1, !multiChain"
          SecRule TX:foo "@unconditionalMatch"  "setvar:tx.test=+1"
    )";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(nullptr, nullptr, 0);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 1);
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
        log, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VARS_NAMES}=%{MATCHED_VARS} %{MATCHED_VAR_NAME}=%{MATCHED_VAR}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 4);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=4");
    // The first matched variable is TX:foo3, and last matched variable is TX:foo4.
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo3=bar TX:foo4=bar");
  }

  // Test that the except variable is specified by the regex.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecAction "phase:1,setvar:tx.foo1=bar,setvar:tx.foo2=bar123,setvar:tx.foo3=bar,setvar:tx.foo4=BAR"
        SecRule TX:foo1|TX:foo2|TX:foo3|TX:foo4|!TX:/.+1/|TX  "@streq bar" \
        "id:1, \
        phase:1, \
        pass, \
        log, \
        t:none, \
        t:lowercase, \
        msg:'tx.test=%{tx.test}', \
        logdata:'%{MATCHED_VARS_NAMES}=%{MATCHED_VARS} %{MATCHED_VAR_NAME}=%{MATCHED_VAR}', \
        setvar:tx.test=+1")";

    Engine engine(spdlog::level::trace);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());

    bool matched = false;
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 4);
    EXPECT_TRUE(matched);
    EXPECT_EQ(t->getMsgMacroExpanded(), "tx.test=4");
    // The first matched variable is TX:foo3, and last matched variable is TX:foo4.
    EXPECT_EQ(t->getLogDataMacroExpanded(), "TX:foo3=bar TX:foo4=bar");
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
        log, \
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
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    EXPECT_FALSE(t->hasVariable("", "test"));
    EXPECT_FALSE(matched);
  }
}

TEST(RuleEvaluateLogicTest, MatchedVarPush) {
  // Test the NEED_PUSH_MATCHED flag of the rule and chained rule is set correctly.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecRule ARGS "@streq test" \
        "id:1, \
        phase:1, \
        pass, \
        log, \
        t:none, \
        t:lowercase, \
        chain"
          SecRule &MATCHED_VARS "@eq 1" \
          "t:none,chain"
            SecRule ARGS "@streq test1" \
            "t:none,t:lowercase,setvar:tx.test=1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    t->processUri("/index.php?id=test&user=test1", "GET", "HTTP/1.1");
    ASSERT_TRUE(result.has_value());

    auto rule = engine.findRuleById(1);
    ASSERT_NE(rule, nullptr);
    EXPECT_TRUE(rule->isNeedPushMatched());

    Rule* child = (const_cast<Rule*>(rule))->chainRule(0);
    ASSERT_NE(child, nullptr);
    // The child rule's NEED_PUSH_MATCHED flag should be false
    EXPECT_FALSE(child->isNeedPushMatched());
    ASSERT_NE(child->chainRule(0), nullptr);
    EXPECT_FALSE(child->chainRule(0)->isNeedPushMatched());

    bool matched = false;
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          bool* matched = static_cast<bool*>(user_data);
          *matched = true;
        },
        &matched);
    // The rule is matched, and the action is executed.
    EXPECT_EQ(std::get<int64_t>(t->getVariable("", "test")), 1);
    EXPECT_TRUE(matched);
  }

  // Test the NEED_PUSH_MATCHED flag of the rule and chained rule is set correctly when only the
  // parent rule uses MATCHED_VAR.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecRule MATCHED_VAR "@streq test" \
        "id:1, \
        phase:1, \
        pass, \
        log, \
        t:none, \
        t:lowercase, \
        chain"
          SecRule ARGS "@rx test" \
          "setvar:tx.test=1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    engine.init();
    auto t = engine.makeTransaction();
    ASSERT_TRUE(result.has_value());
    auto rule = engine.findRuleById(1);
    ASSERT_NE(rule, nullptr);
    // The parent rule's NEED_PUSH_MATCHED flag should be true
    EXPECT_TRUE(rule->isNeedPushMatched());

    Rule* child = (const_cast<Rule*>(rule))->chainRule(0);
    ASSERT_NE(child, nullptr);
    // The child rule's NEED_PUSH_MATCHED flag should be false
    EXPECT_FALSE(child->isNeedPushMatched());
  }

  // Test the NEED_PUSH_MATCHED flag of the rule and chained rule is set correctly when only the
  // parent rule's msg and logdata use MATCHED_VAR.
  {
    const std::string directive = R"(
        SecRuleEngine On
        SecRule ARGS "@streq test" \
        "id:1, \
        phase:1, \
        pass, \
        log, \
        msg:'msg:%{MATCHED_VAR_NAME}=%{MATCHED_VAR}',\
        logdata:'logdata:%{MATCHED_VAR_NAME}=%{MATCHED_VAR}'
        t:none, \
        t:lowercase, \
        chain"
          SecRule ARGS "@rx test" \
          "setvar:tx.test=1")";

    Engine engine(spdlog::level::off);
    auto result = engine.load(directive);
    ASSERT_TRUE(result.has_value());
    engine.init();

    auto rule = engine.findRuleById(1);
    ASSERT_NE(rule, nullptr);
    EXPECT_TRUE(rule->isNeedPushMatched());

    Rule* child = (const_cast<Rule*>(rule))->chainRule(0);
    ASSERT_NE(child, nullptr);
    EXPECT_FALSE(child->isNeedPushMatched());

    auto t = engine.makeTransaction();
    t->processUri("/index.php?id=test&user=test1", "GET", "HTTP/1.1");
    t->processRequestHeaders(
        nullptr, nullptr, 0,
        [](const Rule& rule, void* user_data) {
          Transaction* t = static_cast<Transaction*>(user_data);
          EXPECT_EQ(t->getMsgMacroExpanded(), "msg:ARGS:id=test");
          EXPECT_EQ(t->getLogDataMacroExpanded(), "logdata:ARGS:id=test");
        },
        t.get());
  }
}

} // namespace Integration
} // namespace Wge