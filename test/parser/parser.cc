#include "parser/parser.h"

#include <string>
#include <vector>

#include <gtest/gtest.h>

class ParserTest : public testing::Test {
public:
  static const std::vector<std::string> test_files_;
};

// clang-format off
const std::vector<std::string> ParserTest::test_files_({
  "waf-conf/base/before.conf", 
  "waf-conf/coreruleset/rules/REQUEST-913-SCANNER-DETECTION.conf",
  "waf-conf/coreruleset/rules/REQUEST-920-PROTOCOL-ENFORCEMENT.conf",
  "waf-conf/coreruleset/rules/REQUEST-921-PROTOCOL-ATTACK.conf",
  "waf-conf/coreruleset/rules/REQUEST-922-MULTIPART-ATTACK.conf",
  "waf-conf/coreruleset/rules/REQUEST-930-APPLICATION-ATTACK-LFI.conf",
  "waf-conf/coreruleset/rules/REQUEST-931-APPLICATION-ATTACK-RFI.conf",
  "waf-conf/coreruleset/rules/REQUEST-932-APPLICATION-ATTACK-RCE.conf",
  "waf-conf/coreruleset/rules/REQUEST-933-APPLICATION-ATTACK-PHP.conf",
  "waf-conf/coreruleset/rules/REQUEST-934-APPLICATION-ATTACK-GENERIC.conf",
  "waf-conf/coreruleset/rules/REQUEST-941-APPLICATION-ATTACK-XSS.conf",
  "waf-conf/coreruleset/rules/REQUEST-942-APPLICATION-ATTACK-SQLI.conf",
  "waf-conf/coreruleset/rules/REQUEST-943-APPLICATION-ATTACK-SESSION-FIXATION.conf",
  "waf-conf/coreruleset/rules/REQUEST-944-APPLICATION-ATTACK-JAVA.conf",
  "waf-conf/coreruleset/rules/REQUEST-949-BLOCKING-EVALUATION.conf",
  "waf-conf/coreruleset/rules/RESPONSE-950-DATA-LEAKAGES.conf",
  "waf-conf/coreruleset/rules/RESPONSE-951-DATA-LEAKAGES-SQL.conf",
  "waf-conf/coreruleset/rules/RESPONSE-952-DATA-LEAKAGES-JAVA.conf",
  "waf-conf/coreruleset/rules/RESPONSE-953-DATA-LEAKAGES-PHP.conf",
  "waf-conf/coreruleset/rules/RESPONSE-954-DATA-LEAKAGES-IIS.conf",
  "waf-conf/coreruleset/rules/RESPONSE-955-WEB-SHELLS.conf",
  "waf-conf/base/after.conf"});
// clang-format on

TEST_F(ParserTest, Empty) {
  using namespace SrSecurity::Parser;

  const std::string directive = R"()";

  Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}

TEST_F(ParserTest, Comment) {
  using namespace SrSecurity::Parser;

  const std::string directive = R"(# This is comment1
  # This is comment2
  # This is comment3)";

  Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}

TEST_F(ParserTest, Include) {
  using namespace SrSecurity::Parser;

  const std::string directive = R"(# test include directive
  Include "test/test_data/include_test.conf"
  )";

  Parser parser;
  std::string error = parser.load(directive);
  if (!error.empty()) {
    std::cout << error << std::endl;
  }

  ASSERT_TRUE(error.empty());
}