#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "action/action_base.h"
#include "http_extractor.h"
#include "operator/operator_base.h"
#include "variable/variable_base.h"

namespace SrSecurity {
/**
 * The rule class.
 */
class Rule {
  friend class ParserTest;

public:
  /**
   * Evaluate the rule
   * @return True if intervening
   */
  bool evaluate(const HttpExtractor& extractor) const;

  /**
   * Each rule must call this method once before call evaluate, and only once in the life of the
   * instance.
   * This method initializes some important variables, such as hyperscan database and so on
   */
  void preEvaluate();

public:
  /**
   * Get the rule id
   * @return The value that defined in rule action. E.g: "id: 123456"
   */
  uint64_t id() const { return id_; }

  /**
   * Checks whether this rule contains the specifc tag
   * @param tag Defined in rule action. E.g: "tag: xxx"
   */
  bool hasTag(const std::string& tag) const;

  /**
   * Checks whether this rule contains the specifc tag set
   * @param tags Defined in rule action. E.g: "tag: xxx"
   */
  bool hasTag(const std::unordered_set<std::string>& tags) const;

  void appendVariable(std::unique_ptr<Variable::VariableBase>&& var);
  void setOperator(std::unique_ptr<Operator::OperatorBase>&& op);
  void appendAction(std::unique_ptr<Action::ActionBase>&& action);

private:
  void initHyperscan() {}
  void initPcre() {}

private:
  std::vector<std::unique_ptr<Variable::VariableBase>> variables_pool_;
  std::unique_ptr<Operator::OperatorBase> operator_;
  std::vector<std::unique_ptr<Action::ActionBase>> actions_pool_;

  // Build the map to quick find
  std::unordered_map<std::string, Variable::VariableBase&> variables_map_;
  std::unordered_map<std::string, Action::ActionBase&> actions_map_;

  // Even if the values listed below can be found in the actions_map_, hold these values or
  // reference to get their quickly
private:
  uint64_t id_{0};
  int phase_{-1};
};
} // namespace SrSecurity