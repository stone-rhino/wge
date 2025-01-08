#pragma once

#include <memory>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "action/set_var.h"
#include "http_extractor.h"
#include "operator/operator_base.h"
#include "variable/variable_base.h"

namespace SrSecurity {

namespace Antlr4 {
class Parser;
}

namespace Parser {
class RuleTest;
}

/**
 * The rule class.
 */
class Rule {
  friend class Parser::RuleTest;
  friend class Antlr4::Parser;

public:
  /**
   * Evaluate the rule
   * @return True if intervening
   */
  bool evaluate(const HttpExtractor& extractor) const;

public:
  enum class Severity { EMERGENCY = 0, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG };
  enum class Disruptive {
    // Stops rule processing on a successful match and allows the transaction to proceed.
    ALLOW,
    // Performs the disruptive action defined by the previous SecDefaultAction.
    BLOCK,
    // Stops rule processing and intercepts transaction.
    DENY,
    // Unlike in v2, in ModSecurity v3 this action currently functions the same as the deny action.
    DROP,
    // Continues processing with the next rule in spite of a successful match.
    PASS,
    // Intercepts transaction by issuing an external (client-visible) redirection to the given
    // location..
    REDIRECT
  };

public:
  uint64_t id() const { return id_; }
  int phase() const { return phase_; }
  const Severity severity() const { return severity_; }
  const std::string& msg() const { return msg_; }
  const std::unordered_set<std::string>& tags() const { return tags_; }
  const std::string& ver() const { return ver_; }
  const std::string& rev() const { return rev_; }
  int accuracy() const { return accuracy_; }
  int maturity() const { return maturity_; }

private:
  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);
  void removeVariable(const std::string& full_name);
  void setOperator(std::unique_ptr<Operator::OperatorBase>&& op);

private:
  std::vector<std::unique_ptr<Variable::VariableBase>> variables_pool_;
  std::unique_ptr<Operator::OperatorBase> operator_;

  // Build the index to quick find
  std::unordered_map<std::string_view, Variable::VariableBase&> variables_index_by_full_name_;

  // Action Group: Meta-data
private:
  // Assigns a unique, numeric ID to the rule or chain in which it appears.
  uint64_t id_;

  // Places the rule or chain into one of five available processing phases. It can also be used in
  // SecDefaultAction to establish the rule defaults for that phase.
  int phase_;

  // Assigns severity to the rule in which it is used.
  // Severity values in ModSecurity follows the numeric scale of syslog (where 0 is the most
  // severe):
  // 0 - EMERGENCY
  // 1 - ALERT
  // 2 - CRITICAL
  // 3 - ERROR
  // 4 - WARNING
  // 5 - NOTICE
  // 6 - INFO
  // 7 - DEBUG
  Severity severity_;

  // Assigns a custom message to the rule or chain in which it appears. The message will be logged
  // along with every alert.
  std::string msg_;

  // Assigns a tag (category) to a rule or a chain.
  std::unordered_set<std::string> tags_;

  // Specifies the rule set version.
  std::string ver_;

  // Specifies rule revision. It is useful in combination with the id action to provide an
  // indication that a rule has been changed.
  std::string rev_;

  // Specifies the relative accuracy level of the rule related to false positives/negatives. The
  // value is a string based on a numeric scale (1-9 where 9 is very strong and 1 has many false
  // positives).
  int accuracy_;

  // Specifies the relative maturity level of the rule related to the length of time a rule has been
  // public and the amount of testing it has received. The value is a string based on a numeric
  // scale (1-9 where 9 is extensively tested and 1 is a brand new experimental rule).
  int maturity_;

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
  // std::string set_var_;
  std::unique_ptr<Action::SetVar> set_var_;
  std::unordered_set<std::string> t_;

  // Action Group: Flow
private:
  // Chains the current rule with the rule that immediately follows it, creating a rule chain.
  // Chained rules allow for more complex processing logic.
  bool chain_{false};

  // Skips one or more rules (or chains) on successful match.
  int skip_{0};

  // Skips one or more rules (or chains) on a successful match, resuming rule execution with the
  // first rule that follows the rule (or marker created by SecMarker) with the provided ID.
  std::string skip_after_;

  // Action Group: Disruptive
private:
  Disruptive disruptive_;

  // Intercepts transaction by issuing an external (client-visible) redirection to the given
  // location.
  std::string redirect_;

  // Action Group: Data
private:
  // Specifies the response status code to use with actions deny and redirect.
  std::string status_;

  // Configures an XML namespace, which will be used in the execution of XPath expressions.
  std::string xml_ns_;
};
} // namespace SrSecurity