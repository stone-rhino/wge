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
#include "rule.h"

#include <list>

#include "common/assert.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"
#include "operator/operator_include.h"
#include "variable/collection_base.h"

namespace Wge {
std::unordered_set<std::string> Rule::string_pool_;
void Rule::initExceptVariables() {
  ASSERT_IS_MAIN_THREAD();

  // Traverse the except variables and remove the matched variables from the variables list, or add
  // the except variable to the collection except list.
  for (auto& except_var : detail_->except_variables_) {
    auto except_var_name = except_var->fullName();

    // Init the except scanner
    std::unique_ptr<Common::Pcre::Scanner> except_scanner;
    if (!except_var_name.sub_name_.empty() && except_var_name.sub_name_.front() == '/' &&
        except_var_name.sub_name_.back() == '/') {
      except_scanner = std::make_unique<Common::Pcre::Scanner>(
          std::string_view{except_var_name.sub_name_.data() + 1,
                           except_var_name.sub_name_.size() - 2},
          false, false);
    }

    for (auto iter = variables_.begin(); iter != variables_.end();) {
      auto var_name = (*iter)->fullName();

      // They are not the same collection
      if (var_name.main_name_ != except_var_name.main_name_) {
        ++iter;
        continue;
      }

      // The specific exception is collection or they are the same variable, we remove the variable
      // directly for the performance.
      if (except_var_name.sub_name_.empty() || var_name.sub_name_ == except_var_name.sub_name_) {
        detail_->variables_index_by_full_name_.erase(var_name);
        iter = variables_.erase(iter);
        continue;
      }

      // The specific exception is a regex, if matched, we remove the variable directly
      if (!var_name.sub_name_.empty() && except_scanner &&
          except_scanner->match(var_name.sub_name_)) {
        detail_->variables_index_by_full_name_.erase(var_name);
        iter = variables_.erase(iter);
        continue;
      }

      // The specific exception is a variable and the variable that will be evaluate is a
      // collection, we add the exception variable to the collection except list. It's use for
      // except the specific variable when the collection is evaluated.
      if (var_name.sub_name_.empty() && !except_var_name.sub_name_.empty()) {
        Variable::CollectionBase* collection = dynamic_cast<Variable::CollectionBase*>(iter->get());
        if (collection) {
          collection->addExceptVariable(except_var_name.sub_name_);
        }
      }

      ++iter;
    }
  }
}

void Rule::initPmfOperator(const std::string& serialize_dir) {
  ASSERT_IS_MAIN_THREAD();

  for (auto& op : operators_) {
    Operator::PmFromFile* pm_from_file = dynamic_cast<Operator::PmFromFile*>(op.get());
    if (pm_from_file) {
      pm_from_file->init(serialize_dir);
    }
  }

  // init the pmf operator of chained rule
  if (chain_) {
    chain_->initPmfOperator(serialize_dir);
  }
}

void Rule::initFlags(const Rule& default_action_rule) {
  ASSERT_IS_MAIN_THREAD();

  // Initialize the flags according to the default action rule
  auditLog((default_action_rule.auditLog() || auditLog()) && !noAuditLog());
  log((default_action_rule.log() || log()) && !noLog());
  capture(default_action_rule.capture() || capture());
  multiMatch(default_action_rule.multiMatch() || multiMatch());
}

/**
 * The evaluation process is as follows:
 * 1. Evaluate the variables
 *    - If the variable is a collection, evaluated each element.
 * 2. Evaluate the transformations
 *    - Evaluate the default transformations and the transformation that defined in the rule.
 * 3. Evaluate the operator
 *    - Before evaluating the operator, the variable value was transformed by the transformations.
 *    - If any variable is matched, the rule is matched.
 *    - If any variable is matched, the remaining variables will be evaluated always.
 * 4. Evaluate the actions
 *    - Evaluate the default actions and the action that defined in the rule when each variable is
 *      matched.
 * 5. Evaluate the chained rules
 *    - The chained rule evaluated after the all variables of the rule that prev aspect of the
 *      evaluation process are evaluated.
 *    - Any chained rule is not matched, the rule is not matched, and the remaining chained rules
 *      will not be evaluated.
 * 6. Evaluate the msg macro
 * 7. Evaluate the logdata macro
 */
bool Rule::evaluate(Transaction& t) const {
  WGE_LOG_TRACE("------------------------------------");

  // Check whether the rule is unconditional(SecAction)
  if (operators_.empty())
    [[unlikely]] {
      WGE_LOG_TRACE("evaluate SecAction. id: {} [{}:{}]", id(), filePath(), line());
      // Evaluate the actions
      for (auto& action : matched_branch_actions_) {
        action->evaluate(t);
      }
      return true;
    }

  WGE_LOG_TRACE("evaluate SecRule. id: {} [{}:{}]", id(), filePath(), line());

  // If the multi match is enabled, then perform multiple operator invocations for every target,
  // before and after every anti-evasion transformation is performed.
  if (multiMatch())
    [[unlikely]] {
      WGE_LOG_TRACE("multi match is enabled");
      return evaluateWithMultiMatch(t);
    }

  static thread_local Common::EvaluateElement transformed_value;
  static thread_local std::list<const Transformation::TransformBase*> transform_list;
  static thread_local Operator::OperatorBase::Results op_results;

  // Evaluate the variables
  bool rule_matched = false;
  for (auto& var : variables_) {
    Common::EvaluateResults result;
    evaluateVariable(t, var, result);

    // Evaluate each variable result
    for (size_t i = 0; i < result.size(); ++i) {
      const Common::EvaluateElement& variable_value = result[i];
      transformed_value.clear();
      transform_list.clear();
      if (IS_STRING_VIEW_VARIANT(variable_value.variant_))
        [[likely]] {
          // Evaluate the transformations
          evaluateTransform(t, var.get(), variable_value, transformed_value, transform_list);
        }

      // Evaluate the operator
      op_results.clear();
      evaluateOperator(
          t, transform_list.empty() ? variable_value.variant_ : transformed_value.variant_, var,
          op_results);
      assert(!op_results.empty());

      for (auto& op_result : op_results) {
        // If the variable is matched, evaluate the actions
        if (op_result.matched_) {
          WGE_LOG_TRACE([&]() {
            if (!var->isCollection()) {
              return std::format("variable is matched. {}{}", var->mainName(),
                                 var->subName().empty() ? "" : "." + var->subName());
            } else {
              return std::format("variable of collection is matched. {}:{}", var->mainName(),
                                 variable_value.variable_sub_name_);
            }
          }());

          if (isNeedPushMatched()) {
            t.pushMatchedVariable(var.get(), chain_index_, result[i], transformed_value,
                                  op_result.capture_, std::move(transform_list));
          }

          if (variable_value.ptree_node_) {
            t.pushMatchedVPTree(chain_index_, variable_value.ptree_node_);
          }

          rule_matched = true;

          // Evaluate the matched branch actions
          evaluateActions(t, Action::ActionBase::Branch::Matched);

          // Evaluate the chained rules
          if (chain_ && matchedMultiChain())
            [[unlikely]] { rule_matched = evaluateChain(t); }

          // If the first match is enabled, stop evaluating the rule
          if (firstMatch())
            [[unlikely]] {
              WGE_LOG_TRACE("first match is enabled, stop evaluating the rule");
              break;
            }
        } else {
          // Evaluate the unmatched branch actions
          evaluateActions(t, Action::ActionBase::Branch::Unmatched);

          // Evaluate the chained rules
          if (chain_ && unmatchedMultiChain())
            [[unlikely]] { rule_matched = evaluateChain(t); }
        }
      }

      if (firstMatch() && rule_matched)
        [[unlikely]] { break; }
    }
    if (firstMatch() && rule_matched)
      [[unlikely]] { break; }
  }

  // Evaluate the chained rules
  if (chain_ && !matchedMultiChain() && !unmatchedMultiChain())
    [[unlikely]] {
      if ((rule_matched && matchedChain()) || (!rule_matched && unmatchedChain())) {
        rule_matched = evaluateChain(t);
      }
    }

  return rule_matched;
}

void Rule::appendAction(std::unique_ptr<Action::ActionBase>&& action) {
  ASSERT_IS_MAIN_THREAD();
  detail_->actions_.emplace_back(std::move(action));
  switch (detail_->actions_.back()->branch()) {
  case Action::ActionBase::Branch::Matched:
    matched_branch_actions_.emplace_back(detail_->actions_.back().get());
    break;
  case Action::ActionBase::Branch::Unmatched:
    unmatched_branch_actions_.emplace_back(detail_->actions_.back().get());
    break;
  case Action::ActionBase::Branch::Always:
    matched_branch_actions_.emplace_back(detail_->actions_.back().get());
    unmatched_branch_actions_.emplace_back(detail_->actions_.back().get());
    break;
  }
}

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  ASSERT_IS_MAIN_THREAD();

  if (!var->isNot()) {
    auto full_name = var->fullName();
    auto iter = detail_->variables_index_by_full_name_.find(full_name);

    // Not accept the same variable
    if (iter == detail_->variables_index_by_full_name_.end()) {
      variables_.emplace_back(std::move(var));
      detail_->variables_index_by_full_name_.insert({full_name, *variables_.back()});
    }
  } else {
    detail_->except_variables_.emplace_back(std::move(var));
  }
}

void Rule::capture(bool value) {
  for (auto& op : operators_) {
    Operator::Rx* rx = dynamic_cast<Operator::Rx*>(op.get());
    if (rx) {
      rx->capture(value);
    }
  }
  flags_.set(static_cast<size_t>(Flags::CAPTURE), value);
}

void Rule::emptyMatch(bool value) {
  for (auto& op : operators_) {
    op->emptyMatch(value);
  }
  flags_.set(static_cast<size_t>(Flags::EMPTY_MATCH), value);
}

void Rule::appendOperator(std::unique_ptr<Operator::OperatorBase>&& op) {
  ASSERT_IS_MAIN_THREAD();
  operators_.emplace_back(std::move(op));
}

void Rule::appendChainRule(std::unique_ptr<Rule>&& rule) {
  ASSERT_IS_MAIN_THREAD();
  assert(!chain_);
  chain_ = std::move(rule);

  // The chained rule inherits the phase of the parent rule.
  chain_->phase_ = phase_;

  // Sets the chain index and parent rule for the chained rule.
  chain_->detail_->parent_rule_ = this;
  chain_->detail_->top_rule_ = this;
  chain_->chain_index_ = 0;
  Rule* parent = detail_->parent_rule_;
  while (parent) {
    chain_->detail_->top_rule_ = parent;
    parent = parent->detail_->parent_rule_;
    // Update the chain index
    chain_->chain_index_++;
  }
}

Rule* Rule::chainRule(size_t index) {
  Rule* result = nullptr;
  Rule* parent = this;
  for (size_t i = 0; i <= index; ++i) {
    if (parent->chain_) {
      result = parent->chain_.get();
      parent = parent->chain_.get();
    } else {
      break;
    }
  }
  return result;
}

void Rule::evaluateVariable(Transaction& t, const std::unique_ptr<Wge::Variable::VariableBase>& var,
                            Common::EvaluateResults& result) const {
  var->evaluate(t, result);
  WGE_LOG_TRACE([&]() {
    if (!var->isCollection()) {
      return std::format(
          "evaluate variable: {}{}{}{} = {}", var->isNot() ? "!" : "", var->isCounter() ? "&" : "",
          var->mainName(), var->subName().empty() ? "" : ":" + var->subName(),
          result.empty() ? "nil" : VISTIT_VARIANT_AS_STRING(result.front().variant_));
    } else {
      if (var->isCounter()) {
        return std::format(
            "evaluate collection: {}&{} = {}", var->isNot() ? "!" : "", var->mainName(),
            result.empty() ? "nil" : VISTIT_VARIANT_AS_STRING(result.front().variant_));
      } else {
        return std::format("evaluate collection: {}{}", var->isNot() ? "!" : "", var->mainName());
      }
    }
  }());
}

void Rule::evaluateTransform(
    Transaction& t, const Wge::Variable::VariableBase* var, const Common::EvaluateElement& input,
    Common::EvaluateElement& output,
    std::list<const Transformation::TransformBase*>& transform_list) const {
  const Common::EvaluateElement* p_input = &input;

  // Check if the default transformation should be ignored
  if (!isIgnoreDefaultTransform())
    [[unlikely]] {
      // Check that the default action is defined
      const Wge::Rule* default_action = t.getEngine().defaultActions(phase_);
      if (default_action)
        [[unlikely]] {
          // Get the default transformation
          auto& transforms = default_action->transforms();

          // Evaluate the default transformations
          for (auto& transform : transforms) {
            bool ret = transform->evaluate(t, var, *p_input, output);
            if (ret) {
              transform_list.emplace_back(transform.get());
              p_input = &output;
            }
            WGE_LOG_TRACE("evaluate default transformation: {} {}", transform->name(), ret);
          }
        }
    }

  // Evaluate the action defined transformations
  for (auto& transform : transforms_) {
    bool ret = transform->evaluate(t, var, *p_input, output);
    if (ret) {
      transform_list.emplace_back(transform.get());
      p_input = &output;
    }
    WGE_LOG_TRACE("evaluate action defined transformation: {} {}", transform->name(), ret);
  }
}

void Rule::evaluateOperator(Transaction& t, const Common::Variant& var_value,
                            const std::unique_ptr<Wge::Variable::VariableBase>& var,
                            Operator::OperatorBase::Results& results) const {
  std::optional<bool> additional_cond_matched;
  for (auto& op : operators_) {
    op->evaluate(t, var_value, results);
    for (auto& element : results) {
      element.matched_ ^= op->isNot();
      // Call additional condition if defined and just call once
      if (element.matched_ && t.getAdditionalCond() && !additional_cond_matched.has_value() &&
          IS_STRING_VIEW_VARIANT(var_value)) {
        additional_cond_matched =
            t.getAdditionalCond()(*this, *var.get(), std::get<std::string_view>(var_value),
                                  t.getAdditionalCondUserdata());
        WGE_LOG_TRACE("call additional condition: {}", *additional_cond_matched);
        if (*additional_cond_matched == false) {
          break;
        }
      }
    }

    if (additional_cond_matched.has_value() && *additional_cond_matched == false) {
      results.resize(1);
      results.front().matched_ = false;
      break;
    }
  }

  // Set the captured strings to the transaction
  size_t capture_index = 0;
  for (auto& element : results) {
    if (element.matched_ && !element.capture_.empty()) {
      t.setCapture(capture_index++, element.capture_);
    }
  }

  WGE_LOG_TRACE([&]() {
    std::string operators_str;
    for (auto& op : operators_) {
      if (!operators_str.empty()) {
        operators_str += " | ";
      }

      operators_str += std::format("{}@{} {}", op->isNot() ? "!" : "", op->name(),
                                   op->macro() ? op->macro()->literalValue() : op->literalValue());
    }

    return std::format("evaluate operator: {}", operators_str);
  }());
}

bool Rule::evaluateChain(Transaction& t) const {
  bool matched = true;
  assert(chain_);
  WGE_LOG_TRACE("evaluate chained rule. id: {}", chain_->id());
  WGE_LOG_TRACE("↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

  // Set the chained rule as the current evaluate rule
  t.setCurrentEvaluateRule(chain_.get());

  matched = chain_->evaluate(t);

  // Restore the current rule to the transaction
  t.setCurrentEvaluateRule(this);

  return matched;
}

void Rule::evaluateActions(Transaction& t, Action::ActionBase::Branch branch) const {
  if (branch != Action::ActionBase::Branch::Unmatched) {
    // Evaluate the default actions
    const Wge::Rule* default_action = t.getEngine().defaultActions(phase_);
    if (default_action) {
      for (auto& action : default_action->actions()) {
        action->evaluate(t);
      }
    }

    // Evaluate the matched branch actions
    for (auto& action : matched_branch_actions_) {
      action->evaluate(t);
    }
  } else {
    // Evaluate the unmatched branch actions
    for (auto& action : unmatched_branch_actions_) {
      action->evaluate(t);
    }
  }
}

// Normally, variables are inspected only once per rule, and only after all transformation functions
// have been completed. With multiMatch, variables are checked against the operator before and after
// every transformation function that changes the input.
bool Rule::evaluateWithMultiMatch(Transaction& t) const {
  // Get all of the transformations
  std::vector<Transformation::TransformBase*> transforms;
  if (!isIgnoreDefaultTransform()) {
    const Wge::Rule* default_action = t.getEngine().defaultActions(phase_);
    if (default_action) {
      transforms.reserve(default_action->transforms().size());
      for (auto& transform : default_action->transforms()) {
        transforms.emplace_back(transform.get());
      }
    }
  }
  transforms.reserve(transforms.size() + transforms_.size());
  for (auto& transform : transforms_) {
    transforms.emplace_back(transform.get());
  }

  static thread_local Common::EvaluateElement transformed_value;
  static thread_local std::list<const Transformation::TransformBase*> transform_list;
  static thread_local Operator::OperatorBase::Results op_results;

  // Evaluate the variables
  bool rule_matched = false;
  for (auto& var : variables_) {
    Common::EvaluateResults result;
    evaluateVariable(t, var, result);

    size_t curr_transform_index = 0;

    // Evaluate each variable result
    transformed_value.clear();
    transform_list.clear();
    const Common::EvaluateElement* evaluated_value = nullptr;
    for (size_t i = 0; i < result.size();) {
      if (evaluated_value == nullptr) {
        evaluated_value = &result[i];
      }

      // Evaluate the operator
      op_results.clear();
      evaluateOperator(t, evaluated_value->variant_, var, op_results);
      assert(!op_results.empty());

      bool variable_matched = false;
      for (auto& op_result : op_results) {
        // If the variable is matched, evaluate the actions
        if (op_result.matched_) {
          WGE_LOG_TRACE([&]() {
            if (!var->isCollection()) {
              return std::format("variable is matched. {}{}", var->mainName(),
                                 var->subName().empty() ? "" : "." + var->subName());
            } else {
              return std::format("variable of collection is matched. {}:{}", var->mainName(),
                                 evaluated_value->variable_sub_name_);
            }
          }());

          if (isNeedPushMatched()) {
            t.pushMatchedVariable(var.get(), chain_index_, result[i], transformed_value,
                                  op_result.capture_, std::move(transform_list));
          }

          if (evaluated_value->ptree_node_) {
            t.pushMatchedVPTree(chain_index_, evaluated_value->ptree_node_);
          }

          variable_matched = true;
          rule_matched = true;

          // Evaluate the matched branch actions
          evaluateActions(t, Action::ActionBase::Branch::Matched);

          // Evaluate the chained rules
          if (chain_ && matchedMultiChain())
            [[unlikely]] { rule_matched = evaluateChain(t); }

          // If the first match is enabled, stop evaluating the rule
          if (firstMatch())
            [[unlikely]] {
              WGE_LOG_TRACE("first match is enabled, stop evaluating the rule");
              break;
            }
        } else {
          // Evaluate the unmatched branch actions
          evaluateActions(t, Action::ActionBase::Branch::Unmatched);

          // Evaluate the chained rules
          if (chain_ && unmatchedMultiChain())
            [[unlikely]] { rule_matched = evaluateChain(t); }
        }
      }

      if (firstMatch() && rule_matched)
        [[unlikely]] { break; }

      if (variable_matched) {
        // The variable value is matched, evaluate next variable value
        i++;
        curr_transform_index = 0;
        evaluated_value = nullptr;
      } else {
        // The variable value is not matched, evaluate the transformation and try to match again
        if (IS_STRING_VIEW_VARIANT(evaluated_value->variant_))
          [[likely]] {
            // Evaluate the transformation
            bool ret = false;
            while (!ret && curr_transform_index < transforms.size()) {
              ret = transforms[curr_transform_index]->evaluate(t, var.get(), *evaluated_value,
                                                               transformed_value);
              WGE_LOG_TRACE("evaluate transformation: {} {}",
                            transforms[curr_transform_index]->name(), ret);
              curr_transform_index++;
            }

            if (!ret) {
              // All of the transformations have been evaluated, and the variable value is not
              // matched We need to evaluate the next variable value
              i++;
              curr_transform_index = 0;
              evaluated_value = nullptr;
            } else {
              evaluated_value = &transformed_value;
              transform_list.emplace_back(transforms[curr_transform_index - 1]);
            }
          }
        else {
          i++;
          curr_transform_index = 0;
          evaluated_value = nullptr;
        }
      }
    }
    if (firstMatch() && rule_matched)
      [[unlikely]] { break; }
  }

  // Evaluate the chained rules
  if (chain_ && !matchedMultiChain() && !unmatchedMultiChain()) {
    if ((rule_matched && matchedChain()) || (!rule_matched && unmatchedChain())) {
      rule_matched = evaluateChain(t);
    }
  }

  return rule_matched;
}
} // namespace Wge