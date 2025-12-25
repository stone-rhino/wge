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

#include "antlr4/parser.h"
#include "engine.h"
#include "macro/macro_include.h"
#include "variable/variables_include.h"

namespace Wge {
namespace Parser {
class RuleVariableParseTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(RuleVariableParseTest, PTree) {
  {
    const std::string directive =
        R"(SecRule PTREE:config.max_connection|PTREE:config.test "@lt 100" "id:1,phase:1")";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    auto& variables = parser.rules()[0].back().variables();
    EXPECT_EQ(variables.size(), 2);
    const auto* variable = dynamic_cast<const Variable::PTree*>(variables[0].get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->subName(), "config.max_connection");
    variable = dynamic_cast<const Variable::PTree*>(variables[1].get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->subName(), "config.test");
  }

  {
    const std::string directive =
        R"(SecRule PTREE:config.server_list[].address{} "foo" "id:1,phase:1")";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    auto& variables = parser.rules()[0].back().variables();
    EXPECT_EQ(variables.size(), 1);
    const auto* variable = dynamic_cast<const Variable::PTree*>(variables[0].get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->subName(), "config.server_list[].address{}");
  }

  {
    const std::string directive =
        R"(SecRule PTREE:config.server_list[].price "@lt 100" "id:1,phase:1")";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    auto& variables = parser.rules()[0].back().variables();
    EXPECT_EQ(variables.size(), 1);
    const auto* variable = dynamic_cast<const Variable::PTree*>(variables[0].get());
    ASSERT_NE(variable, nullptr);
    EXPECT_EQ(variable->subName(), "config.server_list[].price");
  }
}

TEST_F(RuleVariableParseTest, PTreeMacro) {
  const std::string directive =
      R"(SecRule ARGS "@rx %{PTREE.config.server_list[].domain{}}" "id:1,phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& op = parser.rules()[0].back().operators().front();
  EXPECT_NE(op->macro(), nullptr);
  EXPECT_EQ(op->macro()->literalValue(), "%{PTREE.config.server_list[].domain{}}");
  EXPECT_NE(dynamic_cast<Macro::VariableMacro*>(op->macro().get()), nullptr);
}

TEST_F(RuleVariableParseTest, Tx) {
  const std::string directive = R"(SecTxNamespace hello
  SecRule TX:aaa "foo" "id:1,phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& variables = parser.rules()[0].back().variables();
  EXPECT_EQ(variables.size(), 1);
  const auto* variable = dynamic_cast<const Variable::Tx*>(variables[0].get());
  ASSERT_NE(variable, nullptr);
  EXPECT_EQ(variable->subName(), "aaa");
  EXPECT_EQ(variable->getNamespace(), "hello");
}

TEST_F(RuleVariableParseTest, Gtx) {
  const std::string directive = R"(SecTxNamespace hello
  SecRule GTX:aaa "foo" "id:1,phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& variables = parser.rules()[0].back().variables();
  EXPECT_EQ(variables.size(), 1);
  const auto* variable = dynamic_cast<const Variable::Tx*>(variables[0].get());
  ASSERT_NE(variable, nullptr);
  EXPECT_EQ(variable->subName(), "aaa");
  EXPECT_EQ(variable->getNamespace(), "");
}

TEST_F(RuleVariableParseTest, MatchedOPTree) {
  const std::string directive =
      R"(SecRule MATCHED_OPTREE../../|MATCHED_OPTREE:foo.bar|MATCHED_OPTREE../hello.world|MATCHED_OPTREE "foo" "id:1,phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& variables = parser.rules()[0].back().variables();
  EXPECT_EQ(variables.size(), 4);
  const auto* variable0 = dynamic_cast<const Variable::MatchedOPTree*>(variables[0].get());
  const auto* variable1 = dynamic_cast<const Variable::MatchedOPTree*>(variables[1].get());
  const auto* variable2 = dynamic_cast<const Variable::MatchedOPTree*>(variables[2].get());
  const auto* variable3 = dynamic_cast<const Variable::MatchedOPTree*>(variables[3].get());
  ASSERT_NE(variable0, nullptr);
  ASSERT_NE(variable1, nullptr);
  ASSERT_NE(variable2, nullptr);
  ASSERT_NE(variable3, nullptr);

  EXPECT_EQ(variable0->parentCount(), 2);
  EXPECT_EQ(variable0->subName(), "../../");
  EXPECT_EQ(variable0->paths().size(), 0);

  EXPECT_EQ(variable1->parentCount(), 0);
  EXPECT_EQ(variable1->subName(), "foo.bar");
  EXPECT_EQ(variable1->paths().size(), 2);

  EXPECT_EQ(variable2->parentCount(), 1);
  EXPECT_EQ(variable2->subName(), "../hello.world");
  EXPECT_EQ(variable2->paths().size(), 2);

  EXPECT_EQ(variable3->parentCount(), 0);
  EXPECT_EQ(variable3->subName(), "");
  EXPECT_EQ(variable3->paths().size(), 0);
}

TEST_F(RuleVariableParseTest, MatchedVPTree) {
  const std::string directive =
      R"(SecRule MATCHED_VPTREE../../|MATCHED_VPTREE:foo.bar|MATCHED_VPTREE../hello.world|MATCHED_VPTREE "foo" "id:1,phase:1")";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  auto& variables = parser.rules()[0].back().variables();
  EXPECT_EQ(variables.size(), 4);
  const auto* variable0 = dynamic_cast<const Variable::MatchedVPTree*>(variables[0].get());
  const auto* variable1 = dynamic_cast<const Variable::MatchedVPTree*>(variables[1].get());
  const auto* variable2 = dynamic_cast<const Variable::MatchedVPTree*>(variables[2].get());
  const auto* variable3 = dynamic_cast<const Variable::MatchedVPTree*>(variables[3].get());
  ASSERT_NE(variable0, nullptr);
  ASSERT_NE(variable1, nullptr);
  ASSERT_NE(variable2, nullptr);
  ASSERT_NE(variable3, nullptr);

  EXPECT_EQ(variable0->parentCount(), 2);
  EXPECT_EQ(variable0->subName(), "../../");
  EXPECT_EQ(variable0->paths().size(), 0);

  EXPECT_EQ(variable1->parentCount(), 0);
  EXPECT_EQ(variable1->subName(), "foo.bar");
  EXPECT_EQ(variable1->paths().size(), 2);

  EXPECT_EQ(variable2->parentCount(), 1);
  EXPECT_EQ(variable2->subName(), "../hello.world");
  EXPECT_EQ(variable2->paths().size(), 2);

  EXPECT_EQ(variable3->parentCount(), 0);
  EXPECT_EQ(variable3->subName(), "");
  EXPECT_EQ(variable3->paths().size(), 0);
}

} // namespace Parser
} // namespace Wge