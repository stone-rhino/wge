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

namespace Parser {
class RuleTest;
}

/**
 * The rule class.
 */
class Rule {
  friend class Parser::RuleTest;

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

  enum class TransformBitFlag : uint64_t {
    Base64Decode = 1LL,
    Base64DecodeExt = 1LL << 1,
    Base64Encode = 1LL << 2,
    CmdLine = 1LL << 3,
    CompressWhiteSpace = 1LL << 4,
    CssDecode = 1LL << 5,
    EscapeSeqDecode = 1LL << 6,
    HexDecode = 1LL << 7,
    HexEncode = 1LL << 8,
    HtmlEntityDecode = 1LL << 9,
    JsDecode = 1LL << 10,
    Length = 1LL << 11,
    LowerCase = 1LL << 12,
    Md5 = 1LL << 13,
    None = 1LL << 14,
    NormalisePathWin = 1LL << 15,
    NormalisePath = 1LL << 16,
    NormalizePathWin = 1LL << 17,
    NormalizePath = 1LL << 18,
    ParityEven7Bit = 1LL << 19,
    ParityOdd7Bit = 1LL << 20,
    ParityZero7Bit = 1LL << 21,
    RemoveComments = 1LL << 22,
    RemoveCommentChar = 1LL << 23,
    RemoveNulls = 1LL << 24,
    RemoveWhitespace = 1LL << 25,
    ReplaceComments = 1LL << 26,
    ReplaceNulls = 1LL << 27,
    Sha1 = 1LL << 28,
    SqlHexDecode = 1LL << 29,
    TrimLeft = 1LL << 30,
    TrimRight = 1LL << 31,
    Trim = 1LL << 32,
    UpperCase = 1LL << 33,
    UrlDecodeUni = 1LL << 34,
    UrlDecode = 1LL << 35,
    UrlEncode = 1LL << 36,
    Utf8ToUnicode = 1LL << 37
  };

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
  bool auditLog() const { return audit_log_; }
  void auditLog(bool value) { audit_log_ = value; }
  bool log() const { return log_; }
  void log(bool value) { log_ = value; };
  bool noAuditLog() const { return no_audit_log_; }
  void noAuditLog(bool value) { no_audit_log_ = value; }
  bool noLog() const { return no_log_; }
  void noLog(bool value) { no_log_ = value; }
  bool capture() const { return capture_; }
  void capture(bool value) { capture_ = value; }
  bool multiMatch() const { return multi_match_; }
  void multiMatch(bool value) { multi_match_ = value; }
  bool hasTransform(TransformBitFlag flag) const {
    return transform_flag_ & static_cast<uint64_t>(flag);
  }
  void addTransform(TransformBitFlag flag) { transform_flag_ |= static_cast<uint64_t>(flag); }
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

public:
  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);
  void removeVariable(const Variable::VariableBase::FullName& full_name);
  void setOperator(std::unique_ptr<Operator::OperatorBase>&& op);

private:
  std::vector<std::unique_ptr<Variable::VariableBase>> variables_pool_;
  std::unique_ptr<Operator::OperatorBase> operator_;

  // Build the index to quick find
  std::unordered_map<Variable::VariableBase::FullName, Variable::VariableBase&>
      variables_index_by_full_name_;

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
  std::string ctl_;
  std::string exec_;
  std::string expire_var_;
  std::string init_col_;
  std::string log_data_;
  bool audit_log_{false};
  bool log_{false};
  bool no_audit_log_{false};
  bool no_log_{false};
  bool capture_{false};
  bool multi_match_{false};
  uint64_t transform_flag_{0};
  std::vector<std::unique_ptr<Action::ActionBase>> actions_;

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