#include <functional>
#include <unordered_map>

#include <gtest/gtest.h>

#include "action/actions_include.h"
#include "antlr4/parser.h"
#include "define.h"
#include "engine.h"
#include "variable/variables_include.h"

namespace SrSecurity {
class CrsTest : public testing::Test {
public:
  static std::unordered_map<uint64_t, std::function<void(const SrSecurity::Rule&)>> rule_tests_;

protected:
  Engine engine_;
};

std::unordered_map<uint64_t, std::function<void(const SrSecurity::Rule&)>> CrsTest::rule_tests_;

TEST_F(CrsTest, crs901) {
  auto result = engine_.loadFromFile(
      "test/test_data/waf-conf/coreruleset/rules/REQUEST-901-INITIALIZATION.conf");
  EXPECT_TRUE(result.has_value());
  if (!result.has_value()) {
    std::cout << result.error() << std::endl;
  }

  engine_.init();

  EXPECT_EQ(engine_.parser().auditLogConfig().component_signature_, "OWASP_CRS/4.3.0-dev");

  auto& rules = engine_.parser().rules();
  EXPECT_EQ(rules.size(), 29);

  for (auto& rule : rules) {
    auto iter = rule_tests_.find(rule->id());
    if (iter != rule_tests_.end()) {
      iter->second(*rule);
    }
  }
}

RULE_TEST(901001) {
  // Variables
  EXPECT_EQ(rule.variables().size(), 1);
  EXPECT_NE(nullptr, dynamic_cast<Variable::Tx*>(rule.variables()[0].get()));
  EXPECT_EQ(rule.variables()[0]->subName(), "crs_setup_version");
  EXPECT_TRUE(rule.variables()[0]->isCounter());

  // Operator
  auto& op = rule.getOperator();
  EXPECT_NE(op, nullptr);
  EXPECT_EQ(std::string("eq"), op->name());
  EXPECT_EQ(std::string("0"), op->literalValue());

  // Actions
  EXPECT_EQ(rule.id(), 901001);
  EXPECT_EQ(rule.phase(), 1);
  EXPECT_EQ(rule.disruptive(), Rule::Disruptive::DENY);
  EXPECT_EQ(rule.status(), "500");
  EXPECT_EQ(rule.log(), true);
  EXPECT_EQ(rule.auditLog(), true);
  EXPECT_EQ(rule.msg(),
            "ModSecurity CRS is deployed without configuration! Please copy the "
            "crs-setup.conf.example template to crs-setup.conf, and include the crs-setup.conf "
            "file in your webserver configuration before including the CRS rules. See the INSTALL "
            "file in the CRS directory for detailed instructions");
  EXPECT_NE(rule.tags().find("OWASP_CRS"), rule.tags().end());
  EXPECT_EQ(rule.ver(), "OWASP_CRS/4.3.0-dev");
  EXPECT_EQ(rule.severity(), Rule::Severity::CRITICAL);
}

RULE_TEST(901100) {
  // Variables
  EXPECT_EQ(rule.variables().size(), 1);
  EXPECT_NE(nullptr, dynamic_cast<Variable::Tx*>(rule.variables()[0].get()));
  EXPECT_EQ(rule.variables()[0]->subName(), "inbound_anomaly_score_threshold");
  EXPECT_TRUE(rule.variables()[0]->isCounter());

  // Operator
  auto& op = rule.getOperator();
  EXPECT_NE(op, nullptr);
  EXPECT_EQ(std::string("eq"), op->name());
  EXPECT_EQ(std::string("0"), op->literalValue());

  // Actions
  EXPECT_EQ(rule.id(), 901100);
  EXPECT_EQ(rule.phase(), 1);
  EXPECT_EQ(rule.disruptive(), Rule::Disruptive::PASS);
  EXPECT_EQ(rule.noLog(), true);
  EXPECT_NE(rule.tags().find("OWASP_CRS"), rule.tags().end());
  EXPECT_EQ(rule.ver(), "OWASP_CRS/4.3.0-dev");

  // Actions object
  EXPECT_EQ(rule.actions().size(), 1);
  Action::SetVar* set_var = dynamic_cast<Action::SetVar*>(rule.actions()[0].get());
  EXPECT_NE(set_var, nullptr);
  EXPECT_EQ(set_var->name(), "inbound_anomaly_score_threshold");
  EXPECT_EQ(set_var->value(), "5");
}
} // namespace SrSecurity