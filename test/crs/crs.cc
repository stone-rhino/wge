#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace SrSecurity {
class CrsTest : public testing::Test {
public:
  void SetUp() override { parser_ = engine_.parser_; }

protected:
  Engine engine_;
  std::shared_ptr<Antlr4::Parser> parser_;
};

TEST_F(CrsTest, crs901) {
  auto result = engine_.loadFromFile(
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
  EXPECT_TRUE(result.has_value());
  if (!result.has_value()) {
    std::cout << result.error() << std::endl;
  }

  // EXPECT_EQ(parser_->auditLogConfig().component_signature_, "OWASP_CRS/4.3.0-dev");

  // auto& rules = parser_->rules();
  // EXPECT_EQ(rules.size(), 1);
  // auto& rule = rules.front();
  // EXPECT_EQ(rule->id(), 901001);
  // EXPECT_EQ(rule->phase(), 1);
  // EXPECT_EQ(rule->disruptive(), Rule::Disruptive::DENY);
  // EXPECT_EQ(rule->status(), "500");
  // EXPECT_EQ(rule->log(), true);
  // EXPECT_EQ(rule->auditLog(), true);
  // EXPECT_EQ(rule->msg(),
  //           "ModSecurity CRS is deployed without configuration! Please copy the "
  //           "crs-setup.conf.example template to crs-setup.conf, and include the crs-setup.conf "
  //           "file in your webserver configuration before including the CRS rules. See the INSTALL "
  //           "file in the CRS directory for detailed instructions");
  // EXPECT_NE(rule->tags().find("OWASP_CRS"), rule->tags().end());
  // EXPECT_EQ(rule->ver(), "OWASP_CRS/4.3.0-dev");
  // EXPECT_EQ(rule->severity(), Rule::Severity::CRITICAL);
}
} // namespace SrSecurity