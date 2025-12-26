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
#pragma once

#include <bitset>
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

namespace Wge {

/**
 * The rule class.
 */
class Rule {
public:
  Rule(std::string_view file_path, int line) {
    detail_ = std::make_unique<Detail>();
    detail_->file_path_ = file_path;
    detail_->line_ = line;
  }

public:
  enum class Disruptive : uint8_t {
    // Stops rule processing on a successful match and allows the transaction to proceed.
    ALLOW,
    // Allow will cause the engine to stop processing the current phase. Other phases will continue
    // as normal
    ALLOW_PHASE,
    // Allow will cause the engine to stop processing the current phase. The next phase to be
    // processed will be phase RESPONSE_HEADERS.
    ALLOW_REQUEST,
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

  enum class Severity : uint8_t {
    EMERGENCY = 0,
    ALERT,
    CRITICAL,
    ERROR,
    WARNING,
    NOTICE,
    INFO,
    DEBUG
  };

public:
  /**
   * Initialize the variables that are excepted.
   * We can't auto initialize in the constructor because the except variables are defined by the
   * SecRuleUpdateTargetById/SecRuleUpdateTargetByTag. Theses directive may be defined after the
   * SecRule, So we must manually initialize the except variables after the all directives are
   * loaded. We must call this function and only once before evaluating the rule.
   */
  void initExceptVariables();

  /**
   * Initialize the Pmf operator.
   * We can't auto initialize the Pmf operator in the constructor because it requires the
   * serialize_dir which is specified by SecPmfSerializeDir. The SecPmfSerializeDir directive may be
   * defined after the SecRule, So We must manually initialize the pmf operator after the all
   * directives are loaded. We must call this function and only once before evaluating the rule.
   * @param serialize_dir The serialize directory.
   */
  void initPmfOperator(const std::string& serialize_dir);

  /**
   * Initialize the flags of the rule according to the default action rule.
   * We can't auto initialize in the constructor because the default action rule is defined after
   * the SecRule. So we must manually initialize the flags after the all directives are loaded. If
   * there has no default action rule, we can skip this step.
   * @param default_action_rule The default action rule.
   */
  void initFlags(const Rule& default_action_rule);

  /**
   * Evaluate the rule
   * The evaluation process is as follows:
   * 1. Evaluate the variables
   *    - If the variable is a collection, evaluated each element.
   *    - If any variable is matched, the rule is matched.
   *    - If any variable is matched, the remaining variables will be evaluated always.
   * 2. Evaluate the transformations
   *    - If the variable is matched, evaluate the default transformations and the transformation
   * that defined in the rule.
   * 3. Evaluate the operator
   *    - Before evaluating the operator, the variable result was transformed by the
   * transformations.
   * 4. Evaluate the actions
   *    - If the variable is matched, evaluate the default actions and the action that defined in
   * the rule.
   * 5. Evaluate the chained rules
   *    - The chained rule evaluated after the all variables of the rule that prev aspect of the
   *      evaluation process are evaluated.
   *    - Any chained rule is not matched, the rule is not matched, and the remaining chained rules
   *      will not be evaluated.
   * 6. Evaluate the msg macro
   * 7. Evaluate the logdata macro
   * @return true if the rule is matched, otherwise false.
   */
  bool evaluate(Transaction& t) const;

  // Flags (Hot Data)
public:
  bool auditLog() const { return flags_.test(static_cast<size_t>(Flags::AUDIT_LOG)); }
  void auditLog(bool value) { flags_.set(static_cast<size_t>(Flags::AUDIT_LOG), value); }
  bool noAuditLog() const { return flags_.test(static_cast<size_t>(Flags::NO_AUDIT_LOG)); }
  void noAuditLog(bool value) { flags_.set(static_cast<size_t>(Flags::NO_AUDIT_LOG), value); }
  bool log() const { return flags_.test(static_cast<size_t>(Flags::LOG)); }
  void log(bool value) { flags_.set(static_cast<size_t>(Flags::LOG), value); }
  bool noLog() const { return flags_.test(static_cast<size_t>(Flags::NO_LOG)); }
  void noLog(bool value) { flags_.set(static_cast<size_t>(Flags::NO_LOG), value); }
  bool capture() const { return flags_.test(static_cast<size_t>(Flags::CAPTURE)); }
  void capture(bool value);
  bool multiMatch() const { return flags_.test(static_cast<size_t>(Flags::MULTI_MATCH)); }
  void multiMatch(bool value) { flags_.set(static_cast<size_t>(Flags::MULTI_MATCH), value); }
  bool firstMatch() const { return flags_.test(static_cast<size_t>(Flags::FIRST_MATCH)); }
  void firstMatch(bool value) { flags_.set(static_cast<size_t>(Flags::FIRST_MATCH), value); }
  bool emptyMatch() const { return flags_.test(static_cast<size_t>(Flags::EMPTY_MATCH)); }
  void emptyMatch(bool value);
  bool allMatch() const { return flags_.test(static_cast<size_t>(Flags::ALL_MATCH)); }
  void allMatch(bool value) { flags_.set(static_cast<size_t>(Flags::ALL_MATCH), value); }
  bool matchedChain() const { return flags_.test(static_cast<size_t>(Flags::MATCHED_CHAIN)); }
  void matchedChain(bool value) { flags_.set(static_cast<size_t>(Flags::MATCHED_CHAIN), value); }
  bool unmatchedChain() const { return flags_.test(static_cast<size_t>(Flags::UNMATCHED_CHAIN)); }
  void unmatchedChain(bool value) {
    flags_.set(static_cast<size_t>(Flags::UNMATCHED_CHAIN), value);
  }
  bool matchedMultiChain() const {
    return flags_.test(static_cast<size_t>(Flags::MATCHED_MULTI_CHAIN));
  }
  void matchedMultiChain(bool value) {
    flags_.set(static_cast<size_t>(Flags::MATCHED_MULTI_CHAIN), value);
  }
  bool unmatchedMultiChain() const {
    return flags_.test(static_cast<size_t>(Flags::UNMATCHED_MULTI_CHAIN));
  }
  void unmatchedMultiChain(bool value) {
    flags_.set(static_cast<size_t>(Flags::UNMATCHED_MULTI_CHAIN), value);
  }
  const std::vector<std::unique_ptr<Transformation::TransformBase>>& transforms() const {
    return transforms_;
  }
  std::vector<std::unique_ptr<Transformation::TransformBase>>& transforms() { return transforms_; }
  bool isIgnoreDefaultTransform() const {
    return flags_.test(static_cast<size_t>(Flags::IGNORE_DEFAULT_TRANSFORM));
  }
  void isIgnoreDefaultTransform(bool ignore) {
    flags_.set(static_cast<size_t>(Flags::IGNORE_DEFAULT_TRANSFORM), ignore);
  }
  bool isNeedPushMatched() const {
    return flags_.test(static_cast<size_t>(Flags::NEED_PUSH_MATCHED));
  }
  void isNeedPushMatched(bool need) {
    flags_.set(static_cast<size_t>(Flags::NEED_PUSH_MATCHED), need);
  }

  // Basic Info (Hot Data)
public:
  RulePhaseType phase() const { return phase_; }
  void phase(RulePhaseType value) { phase_ = value; }
  Disruptive disruptive() const { return disruptive_; }
  void disruptive(Disruptive value) { disruptive_ = value; }
  RuleChainIndexType chainIndex() const { return chain_index_; }
  RuleIndexType index() const { return index_; }
  void index(RuleIndexType value) { index_ = value; }
  int16_t skip() const { return skip_; }
  void skip(int16_t value) { skip_ = value; }
  const std::vector<std::unique_ptr<Action::ActionBase>>& actions() const {
    return detail_->actions_;
  }
  const std::vector<const Action::ActionBase*>& matchedBranchActions() const {
    return matched_branch_actions_;
  }
  const std::vector<const Action::ActionBase*>& unmatchedBranchActions() const {
    return unmatched_branch_actions_;
  }
  void appendAction(std::unique_ptr<Action::ActionBase>&& action);
  const std::vector<std::unique_ptr<Variable::VariableBase>>& variables() const {
    return variables_;
  }
  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);
  const std::vector<std::unique_ptr<Variable::VariableBase>>& exceptVariables() const {
    return detail_->except_variables_;
  }
  const std::unordered_map<Variable::FullName, Variable::VariableBase&>& variablesIndex() const {
    return detail_->variables_index_by_full_name_;
  }
  const std::vector<std::unique_ptr<Operator::OperatorBase>>& operators() const {
    return operators_;
  }
  void appendOperator(std::unique_ptr<Operator::OperatorBase>&& op);
  void clearOperators() { operators_.clear(); }
  void appendChainRule(std::unique_ptr<Rule>&& rule);

  /**
   * Get the rule of the chain by index.
   * @param index the relative index of the chain that starts from this rule. Note that the index is
   * not same as the index of the chain that starts form the top rule.
   * @return nullptr if the he index is out of range, otherwise the pointer to the rule of the
   * chain.
   */
  Rule* chainRule(size_t index);

  // Details (Cold Data)
public:
  uint64_t id() const { return detail_->id_; }
  void id(uint64_t value) { detail_->id_ = value; }
  Severity severity() const { return detail_->severity_; }
  void severity(Severity value) { detail_->severity_ = value; }
  uint8_t accuracy() const { return detail_->accuracy_; }
  void accuracy(uint8_t value) { detail_->accuracy_ = value; }
  uint8_t maturity() const { return detail_->maturity_; }
  void maturity(uint8_t value) { detail_->maturity_ = value; }
  int line() const { return detail_->line_; }
  std::string_view filePath() const { return detail_->file_path_; }
  std::string_view msg() const { return detail_->msg_; }
  void msg(std::string&& value) { detail_->msg_ = intern(std::move(value)); }
  void msg(std::unique_ptr<Macro::MacroBase>&& macro) { detail_->msg_macro_ = std::move(macro); }
  const std::unique_ptr<Macro::MacroBase>& msgMacro() const { return detail_->msg_macro_; }
  std::string_view logdata() const { return detail_->log_data_; }
  void logData(std::string&& value) { detail_->log_data_ = intern(std::move(value)); }
  void logData(std::unique_ptr<Macro::MacroBase>&& macro) {
    detail_->log_data_macro_ = std::move(macro);
  }
  const std::unique_ptr<Macro::MacroBase>& logDataMacro() const { return detail_->log_data_macro_; }
  std::string_view redirect() { return detail_->redirect_; }
  void redirect(std::string&& value) { detail_->redirect_ = intern(std::move(value)); }
  std::string_view status() const { return detail_->status_; }
  void status(std::string&& value) { detail_->status_ = intern(std::move(value)); }
  std::string_view xmlns() const { return detail_->xml_ns_; }
  void xmlns(std::string&& value) { detail_->xml_ns_ = intern(std::move(value)); }
  std::string_view ver() const { return detail_->ver_; }
  void ver(std::string&& value) { detail_->ver_ = intern(std::move(value)); }
  std::string_view rev() const { return detail_->rev_; }
  void rev(std::string&& value) { detail_->rev_ = intern(std::move(value)); }
  std::string_view skipAfter() const { return detail_->skip_after_; }
  void skipAfter(std::string&& skip_after) { detail_->skip_after_ = intern(std::move(skip_after)); }
  const std::unordered_set<std::string_view>& tags() const { return detail_->tags_; }
  std::string_view tags(std::string&& tag) {
    return *(detail_->tags_.emplace(intern(std::move(tag))).first);
  }
  const Rule* parentRule() const { return detail_->parent_rule_; }
  Rule* parentRule() { return detail_->parent_rule_; }
  void parentRule(Rule* parent_rule) { detail_->parent_rule_ = parent_rule; }
  const Rule* topRule() const { return detail_->top_rule_; }
  Rule* topRule() { return detail_->top_rule_; }
  void topRule(Rule* top_rule) { detail_->top_rule_ = top_rule; }

public:
  // String interning
  static std::string_view intern(std::string&& str) {
    return *(string_pool_.emplace(std::move(str)).first);
  }

  // Evaluate the rule
private:
  inline void evaluateVariable(Transaction& t,
                               const std::unique_ptr<Wge::Variable::VariableBase>& var,
                               Common::EvaluateResults& result) const;
  inline void
  evaluateTransform(Transaction& t, const Wge::Variable::VariableBase* var,
                    const Common::EvaluateElement& input, Common::EvaluateElement& output,
                    std::list<const Transformation::TransformBase*>& transform_list) const;
  inline void evaluateOperator(Transaction& t, const Common::Variant& var_value,
                               const std::unique_ptr<Wge::Variable::VariableBase>& var,
                               Operator::OperatorBase::Results& results) const;
  inline bool evaluateChain(Transaction& t) const;
  inline void evaluateActions(Transaction& t, Action::ActionBase::Branch branch) const;
  bool evaluateWithMultiMatch(Transaction& t) const;

private:
  enum class Flags {
    // Marks the transaction for logging in the audit log.
    AUDIT_LOG = 0,

    // Indicates that a successful match of the rule should not be used as criteria to determine
    // whether the transaction should be logged to the audit log.
    NO_AUDIT_LOG,

    // Indicates that a successful match of the rule needs to be logged.
    LOG,

    // Prevents rule matches from appearing in both the error and audit logs.
    NO_LOG,

    // When used together with the regular expression operator (@rx), the capture action will create
    // copies of the regular expression captures and place them into the transaction variable
    // collection.
    CAPTURE,

    // If enabled, WGE will perform multiple operator invocations for every target, before
    // and after every anti-evasion transformation is performed.
    MULTI_MATCH,

    // If enabled, WGE will stop evaluating the rule when the first variable value matches.
    FIRST_MATCH,

    // If enabled, and value of operator is a macro that evaluates to empty, the rule will match.
    EMPTY_MATCH,

    // If enabled, the cartesian product of all variable values and all operator values must match
    // for the rule to be considered a match.
    ALL_MATCH,

    // Indicates that the matched branch actions have chain action.
    MATCHED_CHAIN,

    // Indicates that the unmatched branch actions have chain action.
    UNMATCHED_CHAIN,

    // If enabled, WGE will continue evaluating the chained rules when every variable value of the
    // rule is matched. (By default, WGE will evaluating the chained rules after all variable values
    // were evaluated and the rule matched).
    MATCHED_MULTI_CHAIN,

    // Similar to MATCHED_MULTI_CHAIN, but for the unmatched branch.
    UNMATCHED_MULTI_CHAIN,

    // If enabled, WGE will ignore the default transformation for the matched variable.
    IGNORE_DEFAULT_TRANSFORM,

    // Indicates whether the matched variable needs to be pushed to the transaction's
    // matched_variables_.
    // If any action that requires the matched variable is defined or MATCHED_VAR/MATCHED_VAR_NAME
    // variable is used, this flag will be set to true. Otherwise, it will be false.
    // This flag is used to optimize the performance of the rule evaluation.
    NEED_PUSH_MATCHED,

    TOTAL_FLAGS
  };

  // Basic Info (Hot Data)
private:
  std::bitset<static_cast<size_t>(Flags::TOTAL_FLAGS)> flags_;

  // Places the rule or chain into one of five available processing phases. It can also be used in
  // SecDefaultAction to establish the rule defaults for that phase.
  RulePhaseType phase_{-1};

  Disruptive disruptive_{Disruptive::PASS};

  // If this rule is a chain rule, this is the index of the chain. -1 means this rule is not a
  // chain.
  RuleChainIndexType chain_index_{-1};

  // The index of the rule in the phase. -1 means the rule is not in a phase.
  RuleIndexType index_{-1};

  // Skips one or more rules (or chains) on successful match.
  int16_t skip_{0};

  std::vector<std::unique_ptr<Variable::VariableBase>> variables_;
  std::vector<std::unique_ptr<Transformation::TransformBase>> transforms_;
  std::vector<std::unique_ptr<Operator::OperatorBase>> operators_;
  std::vector<const Action::ActionBase*> matched_branch_actions_;
  std::vector<const Action::ActionBase*> unmatched_branch_actions_;

  // Chains the current rule with the rule that immediately follows it, creating a rule chain.
  // Chained rules allow for more complex processing logic.
  std::unique_ptr<Rule> chain_;

  // Details (Cold Data)
private:
  struct Detail {
    // Assigns a unique, numeric ID to the rule or chain in which it appears.
    uint64_t id_{0};

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

    // Specifies the relative accuracy level of the rule related to false positives/negatives. The
    // value is a string based on a numeric scale (1-9 where 9 is very strong and 1 has many false
    // positives).
    uint8_t accuracy_;

    // Specifies the relative maturity level of the rule related to the length of time a rule has
    // been public and the amount of testing it has received. The value is a string based on a
    // numeric scale (1-9 where 9 is extensively tested and 1 is a brand new experimental rule).
    uint8_t maturity_;

    // The line number of the rule in the configuration file.
    int line_;

    // The file path of the rule.
    std::string_view file_path_;

    // Assigns a custom message to the rule or chain in which it appears. The message will be logged
    // along with every alert.
    std::string_view msg_;
    std::unique_ptr<Macro::MacroBase> msg_macro_;

    // Logs a data fragment as part of the alert message.
    std::string_view log_data_;
    std::unique_ptr<Macro::MacroBase> log_data_macro_;

    // Executes an external script supplied as parameter.
    // TODO(zhouyu 2025-11-05): Implement exec action.
    // std::string_view exec_;

    // Configures a collection variable to expire after the given time period (in seconds).
    // TODO(zhouyu 2025-11-05): Implement expire action.
    // std::string_view expire_var_;

    // Initializes a named persistent collection, either by loading data from storage or by creating
    // a new collection in memory.
    // TODO(zhouyu 2025-11-05): Implement initcol action.
    // std::string_view init_col_;

    // Intercepts transaction by issuing an external (client-visible) redirection to the given
    // location.
    std::string_view redirect_;

    // Specifies the response status code to use with actions deny and redirect.
    std::string_view status_;

    // Configures an XML namespace, which will be used in the execution of XPath expressions.
    std::string_view xml_ns_;

    // Specifies the rule set version.
    std::string_view ver_;

    // Specifies rule revision. It is useful in combination with the id action to provide an
    // indication that a rule has been changed.
    std::string_view rev_;

    // Skips one or more rules (or chains) on a successful match, resuming rule execution with the
    // first rule that follows the rule (or marker created by SecMarker) with the provided ID.
    std::string_view skip_after_;

    // Assigns a tag (category) to a rule or a chain.
    std::unordered_set<std::string_view> tags_;

    // If this rule is a chain rule, this is the parent rule of this rule. nullptr means this rule
    // is not a chained rule.
    Rule* parent_rule_{nullptr};

    // If this rule is a chain rule, this is the top rule of the chain. nullptr means this rule is
    // not a chained rule.
    Rule* top_rule_{nullptr};

    // Build the index to quick find
    std::unordered_map<Variable::FullName, Variable::VariableBase&> variables_index_by_full_name_;

    std::vector<std::unique_ptr<Variable::VariableBase>> except_variables_;

    std::vector<std::unique_ptr<Action::ActionBase>> actions_;
  };

  std::unique_ptr<Detail> detail_;

private:
  // The string intern for all strings in the rule
  static std::unordered_set<std::string> string_pool_;
};
} // namespace Wge