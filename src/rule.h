#pragma once

#include <list>
#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

#include "action/action_base.h"
#include "http_extractor.h"
#include "operator/operator_base.h"
#include "transformation/transform_base.h"
#include "variable/variable_base.h"

namespace SrSecurity {

/**
 * The rule class.
 */
class Rule {
public:
  Rule(std::string_view file_path, int line) : file_path_(file_path), line_(line) {}

public:
  /**
   * Evaluate the rule
   * @return True if intervening
   */
  bool evaluate(Transaction& t) const;

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
  std::string_view filePath() const { return file_path_; }
  int line() const { return line_; }

  // Action Group: Meta-data
public:
  uint64_t id() const { return id_; }
  void id(uint64_t value) { id_ = value; }
  int phase() const { return phase_; }
  void phase(int value) { phase_ = value; }
  const Severity severity() const { return severity_; }
  void severity(Severity value) { severity_ = value; }
  const std::string& msg() const { return msg_; }
  void msg(std::string&& value) { msg_ = std::move(value); }
  void msg(std::shared_ptr<Macro::MacroBase> macro) { msg_macro_ = macro; }
  const std::unordered_set<std::string>& tags() const { return tags_; }
  std::unordered_set<std::string>& tags() { return tags_; }
  const std::string& ver() const { return ver_; }
  void ver(std::string&& value) { ver_ = std::move(value); }
  const std::string& rev() const { return rev_; }
  void rev(std::string&& value) { rev_ = std::move(value); }
  int accuracy() const { return accuracy_; }
  void accuracy(int value) { accuracy_ = value; }
  int maturity() const { return maturity_; }
  void maturity(int value) { maturity_ = value; }

  // Action Group: Non-disruptive
public:
  std::optional<bool> auditLog() const { return audit_log_; }
  void auditLog(bool value) { audit_log_ = value; }
  std::optional<bool> log() const { return log_; }
  void log(bool value) { log_ = value; };
  const std::string& logdata() const { return log_data_; }
  void logData(std::string&& value) { log_data_ = std::move(value); }
  void logData(std::shared_ptr<Macro::MacroBase> macro) { log_data_macro_ = macro; }
  std::optional<bool> capture() const { return capture_; }
  void capture(bool value);
  std::optional<bool> multiMatch() const { return multi_match_; }
  void multiMatch(bool value) { multi_match_ = value; }
  const std::vector<std::unique_ptr<Transformation::TransformBase>>& transforms() const {
    return transforms_;
  }
  std::vector<std::unique_ptr<Transformation::TransformBase>>& transforms() { return transforms_; }
  bool isIgnoreDefaultTransform() const { return is_ingnore_default_transform_; }
  void isIgnoreDefaultTransform(bool ignore) { is_ingnore_default_transform_ = ignore; }
  const std::vector<std::unique_ptr<Action::ActionBase>>& actions() const { return actions_; }
  std::vector<std::unique_ptr<Action::ActionBase>>& actions() { return actions_; }

  // Action Group: Disruptive
public:
  Disruptive disruptive() const { return disruptive_; }
  void disruptive(Disruptive value) { disruptive_ = value; }
  const std::string& redirect() { return redirect_; }
  void redirect(std::string&& value) { redirect_ = std::move(value); }

  // Action Group: Data
public:
  const std::string& status() const { return status_; }
  void status(std::string&& value) { status_ = std::move(value); }
  const std::string& xmlns() const { return xml_ns_; }
  void xmlns(std::string&& value) { xml_ns_ = std::move(value); }

  // Action Grop: Flow
public:
  std::list<std::unique_ptr<Rule>>::iterator appendChainRule(int line) {
    chain_.emplace_back(std::make_unique<Rule>(file_path_, line));

    // The chained rule inherits the phase of the parent rule.
    chain_.back()->phase(phase_);

    return std::prev(chain_.end());
  }
  void removeBackChainRule() { chain_.erase(std::prev(chain_.end())); }
  std::unique_ptr<Rule>& backChainRule() { return chain_.back(); }
  int skip() const { return skip_; }
  void skip(int value) { skip_ = value; }
  const std::string& skipAfter() const { return skip_after_; }
  void skipAfter(std::string&& skip_after) { skip_after_ = std::move(skip_after); }

public:
  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);

  void removeVariable(const Variable::VariableBase::FullName& full_name);

  const std::vector<std::unique_ptr<Variable::VariableBase>>& variables() const {
    return variables_;
  }

  const std::unordered_map<Variable::VariableBase::FullName, Variable::VariableBase&>&
  variablesIndex() const {
    return variables_index_by_full_name_;
  }

  void setOperator(std::unique_ptr<Operator::OperatorBase>&& op);
  const std::unique_ptr<Operator::OperatorBase>& getOperator() const { return operator_; }

  // Evaluate the rule
private:
  inline void evaluateVariable(Transaction& t,
                               const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                               Common::EvaluateResult& result) const;
  inline bool
  evaluateDefalutTransform(Transaction& t, const Common::Variant& var_value,
                           const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                           Common::EvaluateResult& result) const;
  inline bool
  evaluateActionTransform(Transaction& t, const Common::Variant& var_value,
                          const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                          Common::EvaluateResult& result) const;
  inline bool evaluateOperator(Transaction& t, const Common::Variant& var_value) const;
  inline bool evaluateChain(Transaction& t) const;
  inline void evaluateMsgMacro(Transaction& t) const;
  inline void evaluateLogDataMacro(Transaction& t) const;
  inline void evaluateActions(Transaction& t) const;

private:
  std::string_view file_path_;
  int line_;
  std::vector<std::unique_ptr<Variable::VariableBase>> variables_;
  std::unique_ptr<Operator::OperatorBase> operator_;

  // Build the index to quick find
  std::unordered_map<Variable::VariableBase::FullName, Variable::VariableBase&>
      variables_index_by_full_name_;

  // Action Group: Meta-data
private:
  // Assigns a unique, numeric ID to the rule or chain in which it appears.
  uint64_t id_{0};

  // Places the rule or chain into one of five available processing phases. It can also be used in
  // SecDefaultAction to establish the rule defaults for that phase.
  int phase_{-1};

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
  std::shared_ptr<Macro::MacroBase> msg_macro_;

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
  std::string exec_;
  std::string expire_var_;
  std::string init_col_;
  std::string log_data_;
  std::shared_ptr<Macro::MacroBase> log_data_macro_;

  std::optional<bool> audit_log_;
  std::optional<bool> log_;
  std::optional<bool> capture_;
  std::optional<bool> multi_match_;
  std::vector<std::unique_ptr<Transformation::TransformBase>> transforms_;
  bool is_ingnore_default_transform_{false};
  std::vector<std::unique_ptr<Action::ActionBase>> actions_;

  // Action Group: Flow
private:
  // Chains the current rule with the rule that immediately follows it, creating a rule chain.
  // Chained rules allow for more complex processing logic.
  std::list<std::unique_ptr<Rule>> chain_;

  // Skips one or more rules (or chains) on successful match.
  int skip_{0};

  // Skips one or more rules (or chains) on a successful match, resuming rule execution with the
  // first rule that follows the rule (or marker created by SecMarker) with the provided ID.
  std::string skip_after_;

  // Action Group: Disruptive
private:
  Disruptive disruptive_{Disruptive::ALLOW};

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