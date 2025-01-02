#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "action/action_base.h"
#include "http_extractor.h"
#include "operator/operator_base.h"
#include "variable/variable_base.h"

namespace SrSecurity {

namespace Antlr4 {
class Parser;
}

/**
 * The rule class.
 */
class Rule {
  friend class ParserTest;
  friend class Antlr4::Parser;

public:
  /**
   * Evaluate the rule
   * @return True if intervening
   */
  bool evaluate(const HttpExtractor& extractor) const;

public:
  enum class Severity { EMERGENCY = 0, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG };
  enum class Disruptive { ALLOW, BLOCK, DENY, PASS, REDIRECT };

public:
  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);
  void setOperator(std::unique_ptr<Operator::OperatorBase>&& op);

public:
  const std::string& accuracy() const { return accuracy_; }
  uint64_t id() const { return id_; }

private:
  std::vector<std::unique_ptr<Variable::VariableBase>> variables_pool_;
  std::unique_ptr<Operator::OperatorBase> operator_;

  // Build the map to quick find
  std::unordered_map<std::string, Variable::VariableBase&> variables_map_;

  // Action Group: Meta-data
private:
  std::string accuracy_;
  uint64_t id_;
  std::string maturity_;
  std::string msg_;
  std::string phase_;
  std::string rev_;
  Severity severity_;
  std::unordered_set<std::string> tag_;
  std::string ver_;

  // Action Group: Non-disruptive
private:
  bool audit_log_{false};
  bool capture_{false};
  std::string ctl_;
  std::string exec_;
  std::string expire_var_;
  std::string init_col_;
  bool log_{false};
  std::string log_data_;
  bool multi_match_{false};
  bool no_audit_log_{false};
  bool no_log_{false};
  std::string set_uid_;
  std::string set_rsc_;
  std::string set_sid_;
  std::string set_env_;
  std::string set_var_;
  std::unordered_set<std::string> t_;

  // Action Group: Flow
private:
  bool chain_{false};
  bool skip_{false};
  std::string skip_after_;

  // Action Group: Disruptive
private:
  Disruptive disruptive_;
  std::string redirect_;

  // Action Group: Data
private:
  std::string status_;
  std::string xml_ns_;
};
} // namespace SrSecurity