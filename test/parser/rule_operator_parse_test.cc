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

namespace Wge {
namespace Parser {
class RuleOperatorParseTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(RuleOperatorParseTest, beginsWith) {
  const std::string directive =
      R"(SecRule TX:foo "@beginsWith ba" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("beginsWith"));
  EXPECT_EQ(op->literalValue(), "ba");
}

TEST_F(RuleOperatorParseTest, beginsWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@beginsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("beginsWith"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, endsWith) {
  const std::string directive =
      R"(SecRule TX:foo "@endsWith ar" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("endsWith"));
  EXPECT_EQ(op->literalValue(), "ar");
}

TEST_F(RuleOperatorParseTest, endsWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@endsWith %{tx.bar}" "id:1,phase:1,setvar:'tx.v1',tag:'foo',msg:'bar'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("endsWith"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, ipMatch) {
  const std::string directive =
      R"(SecRule TX:ipv4 "@ipMatch 192.168.1.1" "id:1,phase:1,setvar:'tx.ipv4_true'"
  SecRule TX:ipv4 "@ipMatch 192.168.1.0/24" "id:2,phase:1,setvar:'tx.ipv4_mark_true'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:370:7349" "id:1,phase:1,setvar:'tx.ipv6_false'"
  SecRule TX:ipv6 "@ipMatch 2001:db8:85a3:8d3:1319:8a2e:270:0000/24" "id:1,phase:1,setvar:'tx.ipv6_mask_false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rules = parser.rules();
  EXPECT_EQ(rules.size(), 4);
  size_t i = 0;
  for (auto& rule : rules) {
    auto& op = rule->getOperator();
    EXPECT_EQ(op->name(), std::string_view("ipMatch"));
    switch (i) {
    case 0:
      EXPECT_EQ(rule->getOperator()->literalValue(), "192.168.1.1");
      break;
    case 1:
      EXPECT_EQ(rule->getOperator()->literalValue(), "192.168.1.0/24");
      break;
    case 2:
      EXPECT_EQ(rule->getOperator()->literalValue(), "2001:db8:85a3:8d3:1319:8a2e:370:7349");
      break;
    case 3:
      EXPECT_EQ(rule->getOperator()->literalValue(), "2001:db8:85a3:8d3:1319:8a2e:270:0000/24");
      break;
    default:
      ASSERT_TRUE(false);
      break;
    }
    ++i;
  }
}

TEST_F(RuleOperatorParseTest, pm) {
  const std::string directive =
      R"(SecRule TX:foo "@pm hello1 world1" "id:5,phase:1,setvar:'tx.false2'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("pm"));
  EXPECT_EQ(op->literalValue(), "hello1 world1");
}

TEST_F(RuleOperatorParseTest, within) {
  const std::string directive =
      R"(SecRule TX:foo "@within hello1 world1" "id:5,phase:1,setvar:'tx.false2'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("within"));
  EXPECT_EQ(op->literalValue(), "hello1 world1");
}

TEST_F(RuleOperatorParseTest, withinWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@within %{tx.v5}" "id:5,phase:1,setvar:'tx.false2'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("within"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, rx) {
  const std::string directive =
      R"(SecRule TX:foo "@rx ^\w+\d+\w+$" "id:1,phase:1,setvar:'tx.true1'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rx"));
  EXPECT_EQ(op->literalValue(), R"(^\w+\d+\w+$)");
}

TEST_F(RuleOperatorParseTest, rxWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@rx %{tx.hello}" "id:1,phase:1,setvar:'tx.true1'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rx"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, pmFromFile) {
  const std::string directive =
      R"(SecRule TX:bar "@pmFromFile test/test_data/pmf_test.data" "id:1,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("pmFromFile"));
  EXPECT_EQ(op->literalValue(), R"(test/test_data/pmf_test.data)");
}

TEST_F(RuleOperatorParseTest, streq) {
  const std::string directive =
      R"(SecRule TX:foo "@streq helloworld1" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("streq"));
  EXPECT_EQ(op->literalValue(), "helloworld1");
}

TEST_F(RuleOperatorParseTest, streqWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@streq %{tx.hello}" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("streq"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, validateUrlEncoding) {
  const std::string directive =
      R"(SecRule TX:bar "@validateUrlEncoding" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("validateUrlEncoding"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, contains) {
  const std::string directive =
      R"(SecRule TX:foo "@contains hello1" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("contains"));
  EXPECT_EQ(op->literalValue(), "hello1");
}

TEST_F(RuleOperatorParseTest, containsWithMacro) {
  const std::string directive =
      R"(SecRule TX:foo "@contains %{tx.foo}" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("contains"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, validateByteRange) {
  const std::string directive =
      R"(SecRule TX:bar "@validateByteRange 65,66-68" "id:2,phase:1,setvar:'tx.false'")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("validateByteRange"));
  EXPECT_EQ(op->literalValue(), "65,66-68");
}

TEST_F(RuleOperatorParseTest, detectSQLiAndSyntaxCheck) {
  const std::string directive = R"(SecRule TX:bar "@detectSQLiAndSyntaxCheck" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("detectSQLiAndSyntaxCheck"));
  EXPECT_EQ(op->literalValue(), "");
}

TEST_F(RuleOperatorParseTest, rxAndSyntaxCheckJava) {
  const std::string directive = R"(SecRule TX:bar "@rxAndSyntaxCheckJava hello" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rxAndSyntaxCheckJava"));
  EXPECT_EQ(op->literalValue(), "hello");
}

TEST_F(RuleOperatorParseTest, rxAndSyntaxCheckJS) {
  const std::string directive = R"(SecRule TX:bar "@rxAndSyntaxCheckJS hello" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rxAndSyntaxCheckJS"));
  EXPECT_EQ(op->literalValue(), "hello");
}

TEST_F(RuleOperatorParseTest, rxAndSyntaxCheckPHP) {
  const std::string directive = R"(SecRule TX:bar "@rxAndSyntaxCheckPHP hello" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rxAndSyntaxCheckPHP"));
  EXPECT_EQ(op->literalValue(), "hello");
}

TEST_F(RuleOperatorParseTest, rxAndSyntaxCheckShell) {
  const std::string directive = R"(SecRule TX:bar "@rxAndSyntaxCheckShell hello" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rxAndSyntaxCheckShell"));
  EXPECT_EQ(op->literalValue(), "hello");
}

TEST_F(RuleOperatorParseTest, rxAndSyntaxCheckSQL) {
  const std::string directive = R"(SecRule TX:bar "@rxAndSyntaxCheckSQL hello" "id:1, phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& rule = parser.rules().back();
  auto& op = rule->getOperator();
  EXPECT_EQ(op->name(), std::string_view("rxAndSyntaxCheckSQL"));
  EXPECT_EQ(op->literalValue(), "hello");
}
} // namespace Parser
} // namespace Wge