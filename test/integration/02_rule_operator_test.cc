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
namespace Integration {
class RuleOperatorTest : public testing::Test {
public:
  RuleOperatorTest() : engine_(spdlog::level::off) {}

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

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, beginsWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar,setvar:tx.bar=ba,setvar:tx.bar1=bar1"
  SecRule TX:foo "@beginsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
  SecRule TX:foo "@beginsWith %{tx.bar1}" "id:2,phase:1,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, endsWith) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar"
      SecRule TX:foo "@endsWith ar" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
      SecRule TX:foo "@endsWith ba" "id:1,phase:2,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("v1"));
  EXPECT_FALSE(t->hasVariable("v2"));
}

TEST_F(RuleOperatorTest, endsWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=bar,setvar:tx.bar=ar,setvar:tx.bar1=bar1"
  SecRule TX:foo "@endsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'"
  SecRule TX:foo "@endsWith %{tx.bar1}" "id:2,phase:1,setvar:'tx.v2',tag:'foo',msg:'bar'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
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

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("ipv4_true"));
  EXPECT_FALSE(t->hasVariable("ipv4_false"));
  EXPECT_TRUE(t->hasVariable("ipv4_mark_true"));
  EXPECT_FALSE(t->hasVariable("ipv4_mark_false"));
  EXPECT_TRUE(t->hasVariable("ipv6_true"));
  EXPECT_FALSE(t->hasVariable("ipv6_false"));
  EXPECT_TRUE(t->hasVariable("ipv6_mask_true"));
  EXPECT_FALSE(t->hasVariable("ipv6_mask_false"));
}

TEST_F(RuleOperatorTest, pm) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld"
      SecRule TX:foo "@pm hello" "id:1,phase:1,setvar:'tx.true1'"
      SecRule TX:foo "@pm hello " "id:2,phase:1,setvar:'tx.true2'"
      SecRule TX:foo "@pm hello1 world" "id:3,phase:1,setvar:'tx.true3'"
      SecRule TX:foo "@pm hello1" "id:4,phase:1,setvar:'tx.false1'"
      SecRule TX:foo "@pm hello1 world1" "id:5,phase:1,setvar:'tx.false2'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_TRUE(t->hasVariable("true3"));
  EXPECT_FALSE(t->hasVariable("false1"));
  EXPECT_FALSE(t->hasVariable("false2"));
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

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
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

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_TRUE(t->hasVariable("true3"));
  EXPECT_TRUE(t->hasVariable("true4"));
  EXPECT_FALSE(t->hasVariable("false1"));
  EXPECT_FALSE(t->hasVariable("false2"));
}

TEST_F(RuleOperatorTest, rx) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld123helloworld"
  SecRule TX:foo "@rx ^\w+\d+\w+$" "id:1,phase:1,setvar:'tx.true1'"
  SecRule TX:foo "^\w+\d+\w+$" "id:1,phase:1,setvar:'tx.true2'"
  SecRule TX:foo "@rx ^\d+$" "id:1,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, rxWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld123helloworld"
  SecAction "phase:1,setvar:tx.pattern=^\w+\d+\w+$"
  SecAction "phase:1,setvar:tx.pattern_w=\w"
  SecAction "phase:1,setvar:tx.pattern_d=\d"
  SecRule TX:foo "@rx %{tx.pattern}" "id:1,phase:1,setvar:'tx.true1'"
  SecRule TX:foo "%{tx.pattern}" "id:2,phase:1,setvar:'tx.true2'"
  SecRule TX:foo "^%{tx.pattern_w}+%{tx.pattern_d}+%{tx.pattern_w}+$" "id:3,phase:1,setvar:'tx.true3'"
  SecRule TX:foo "@rx ^\d+$" "id:1,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true1"));
  EXPECT_TRUE(t->hasVariable("true2"));
  EXPECT_TRUE(t->hasVariable("true3"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, pmFromFile) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=com.autoregister_verbose,setvar:tx.bar=helloworld"
      SecRule TX:foo "@pmFromFile test/test_data/pmf_test.data" "id:1,phase:1,setvar:'tx.true'"
      SecRule TX:bar "@pmFromFile test/test_data/pmf_test.data" "id:1,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, streq) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld"
  SecRule TX:foo "@streq helloworld" "id:1,phase:1,setvar:'tx.true'"
  SecRule TX:foo "@streq helloworld1" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, streqWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld,setvar:tx.bar=helloword1"
  SecRule TX:foo "@streq %{tx.foo}" "id:1,phase:1,setvar:'tx.true'"
  SecRule TX:foo "@streq %{tx.bar}" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, validateUrlEncoding) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=/asdf%20%ab,setvar:tx.bar=/asdf%20%ag"
  SecRule TX:foo "@validateUrlEncoding" "id:1,phase:1,setvar:'tx.true'"
  SecRule TX:bar "@validateUrlEncoding" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, contains) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld,setvar:tx.bar=hello"
  SecRule TX:foo "@contains hello" "id:1,phase:1,setvar:'tx.true'"
  SecRule TX:foo "@contains hello1" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, containsWithMacro) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=helloworld,setvar:tx.bar=hello"
  SecRule TX:foo "@contains %{tx.bar}" "id:1,phase:1,setvar:'tx.true'"
  SecRule TX:foo "@contains %{tx.bar}1" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}

TEST_F(RuleOperatorTest, validateByteRange) {
  const std::string directive =
      R"(SecAction "phase:1,setvar:tx.foo=abcd,setvar:tx.bar=ABCD"
SecRule TX:foo "@validateByteRange 123" "id:1,phase:1,setvar:'tx.true'"
SecRule TX:bar "@validateByteRange 65,66-68" "id:2,phase:1,setvar:'tx.false'")";

  auto result = engine_.load(directive);
  engine_.init();
  auto t = engine_.makeTransaction();
  ASSERT_TRUE(result.has_value());

  t->processRequestHeaders(nullptr, nullptr, 0, nullptr);
  EXPECT_TRUE(t->hasVariable("true"));
  EXPECT_FALSE(t->hasVariable("false"));
}
} // namespace Integration
} // namespace SrSecurity