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

#include <iostream>

#include "common/assert.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"
#include "operator/rx.h"
#include "variable/collection_base.h"

namespace Wge {
void Rule::initExceptVariables() {
  ASSERT_IS_MAIN_THREAD();

  // Traverse the except variables and remove the matched variables from the variables list, or add
  // the except variable to the collection except list.
  for (auto& except_var : except_variables_) {
    auto except_var_name = except_var->fullName();
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
        variables_index_by_full_name_.erase(var_name);
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

/**
 * The evaluation process is as follows:
 * 1. Evaluate the variables
 *    - If the variable is a collection, evaluated each element.
 *    - If any variable is matched, the rule is matched.
 *    - If any variable is matched, the remaining variables will be evaluated always.
 * 2. Evaluate the transformations
 *    - If the variable is matched, evaluate the default transformations and the transformation that
 *      defined in the rule.
 * 3. Evaluate the operator
 *    - Before evaluating the operator, the variable result was transformed by the transformations.
 * 4. Evaluate the actions
 *    - If the variable is matched, evaluate the default actions and the action that defined in the
 *      rule.
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
  bool is_uncondition = operator_ == nullptr;
  if (is_uncondition) [[unlikely]] {
    WGE_LOG_TRACE("evaluate SecAction. id: {} [{}:{}]", id_, file_path_, line_);
    // Evaluate the actions
    for (auto& action : actions_) {
      action->evaluate(t);
    }
    return true;
  }

  WGE_LOG_TRACE("evaluate SecRule. id: {} [{}:{}]", id_, file_path_, line_);

  // If the multi match is enabled, then perform multiple operator invocations for every target,
  // before and after every anti-evasion transformation is performed.
  if (multi_match_.value_or(false)) [[unlikely]] {
    WGE_LOG_TRACE("multi match is enabled");
    return evaluateWithMultiMatch(t);
  }

  // Evaluate the variables
  bool rule_matched = false;
  for (auto& var : variables_) {
    Common::EvaluateResults result;
    evaluateVariable(t, var, result);

    // Evaluate each variable result
    for (size_t i = 0; i < result.size(); i++) {
      Common::EvaluateResults::Element& variable_value = result.get(i);
      bool variable_matched = false;
      if (IS_STRING_VIEW_VARIANT(variable_value.variant_)) [[likely]] {
        // Evaluate the transformations
        evaluateTransform(t, var.get(), variable_value);
      }

      // Evaluate the operator
      variable_matched = evaluateOperator(t, variable_value.variant_);

      // If the variable is matched, evaluate the actions
      if (variable_matched) {
        t.pushMatchedVariable(var.get(), result.move(i));
        WGE_LOG_TRACE([&]() {
          if (!var->isCollection()) {
            return std::format("variable is matched. {}{}", var->mainName(),
                               var->subName().empty() ? "" : "." + var->subName());
          } else {
            auto& matched_var = t.getMatchedVariables().back();
            return std::format("variable of collection is matched. {}:{}", var->mainName(),
                               matched_var.second.variable_sub_name_);
          }
        }());

        rule_matched = true;

        // Evaluate the default actions and the action defined actions
        evaluateActions(t);
      }
    }
  }

  // Evaluate the chained rules
  if (rule_matched) {
    if (!chain_.empty()) [[unlikely]] {
      // If the chained rules are matched means the rule is matched, otherwise the rule is not
      // matched
      if (!evaluateChain(t)) {
        rule_matched = false;
      }
    }
  }

  // Evaluate the msg macro and logdata macro
  if (rule_matched) {
    evaluateMsgMacro(t);
    evaluateLogDataMacro(t);
  }

  return rule_matched;
}

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  ASSERT_IS_MAIN_THREAD();

  if (!var->isNot()) {
    auto full_name = var->fullName();
    auto iter = variables_index_by_full_name_.find(full_name);

    // Not accept the same variable
    if (iter == variables_index_by_full_name_.end()) {
      variables_.emplace_back(std::move(var));
      variables_index_by_full_name_.insert({full_name, *variables_.back()});
    }
  } else {
    except_variables_.emplace_back(std::move(var));
  }
}

void Rule::capture(bool value) {
  Operator::Rx* rx = dynamic_cast<Operator::Rx*>(operator_.get());
  if (rx) {
    rx->capture(value);
  }
  capture_ = value;
}

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) {
  ASSERT_IS_MAIN_THREAD();
  operator_ = std::move(op);
}

inline void Rule::evaluateVariable(Transaction& t,
                                   const std::unique_ptr<Wge::Variable::VariableBase>& var,
                                   Common::EvaluateResults& result) const {
  var->evaluate(t, result);
  WGE_LOG_TRACE([&]() {
    if (!var->isCollection()) {
      return std::format("evaluate variable: {}{}{}{} = {}", var->isNot() ? "!" : "",
                         var->isCounter() ? "&" : "", var->mainName(),
                         var->subName().empty() ? "" : ":" + var->subName(),
                         VISTIT_VARIANT_AS_STRING(result.front().variant_));
    } else {
      if (var->isCounter()) {
        return std::format("evaluate collection: {}&{} = {}", var->isNot() ? "!" : "",
                           var->mainName(), VISTIT_VARIANT_AS_STRING(result.front().variant_));
      } else {
        return std::format("evaluate collection: {}{}", var->isNot() ? "!" : "", var->mainName());
      }
    }
  }());
}

inline void Rule::evaluateTransform(Transaction& t, const Wge::Variable::VariableBase* var,
                                    Common::EvaluateResults::Element& data) const {
  // Check if the default transformation should be ignored
  if (!is_ingnore_default_transform_) [[unlikely]] {
    // Check that the default action is defined
    const Wge::Rule* default_action = t.getEngine().defaultActions(phase_);
    if (default_action) [[unlikely]] {
      // Get the default transformation
      auto& transforms = default_action->transforms();

      // Evaluate the default transformations
      for (auto& transform : transforms) {
        bool ret = transform->evaluate(t, var, data);
        WGE_LOG_TRACE("evaluate default transformation: {} {}", transform->name(), ret);
      }
    }
  }

  // Evaluate the action defined transformations
  for (auto& transform : transforms_) {
    bool ret = transform->evaluate(t, var, data);
    WGE_LOG_TRACE("evaluate action defined transformation: {} {}", transform->name(), ret);
  }
}

inline bool Rule::evaluateOperator(Transaction& t, const Common::Variant& var_value) const {
  bool matched = operator_->evaluate(t, var_value);
  matched = operator_->isNot() ^ matched;
  WGE_LOG_TRACE("evaluate operator: {} {}@{} {} = {}", VISTIT_VARIANT_AS_STRING(var_value),
                operator_->isNot() ? "!" : "", operator_->name(),
                operator_->macro() ? operator_->macro()->literalValue() : operator_->literalValue(),
                matched);
  return matched;
}

inline bool Rule::evaluateChain(Transaction& t) const {
  bool matched = true;
  for (auto& rule : chain_) {
    WGE_LOG_TRACE("evaluate chained rule. id: {}", rule->id());
    WGE_LOG_TRACE("↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
    matched = rule->evaluate(t);
    if (!matched) {
      break;
    }
  }

  return matched;
}

inline void Rule::evaluateMsgMacro(Transaction& t) const {
  if (msg_macro_) [[unlikely]] {
    Common::EvaluateResults msg_result;
    msg_macro_->evaluate(t, msg_result);
    t.setMsgMacroExpanded(msg_result.move(0));
    WGE_LOG_TRACE("evaluate msg macro: {}", t.getMsgMacroExpanded());
  }
}

inline void Rule::evaluateLogDataMacro(Transaction& t) const {
  if (log_data_macro_) [[unlikely]] {
    Common::EvaluateResults log_data_result;
    log_data_macro_->evaluate(t, log_data_result);
    t.setLogDataMacroExpanded(log_data_result.move(0));
    WGE_LOG_TRACE("evaluate logdata macro: {}", t.getLogDataMacroExpanded());
  }
}

inline void Rule::evaluateActions(Transaction& t) const {
  // Evaluate the default actions
  const Wge::Rule* default_action = t.getEngine().defaultActions(phase_);
  if (default_action) {
    for (auto& action : default_action->actions()) {
      action->evaluate(t);
    }
  }

  // Evaluate the action defined actions
  for (auto& action : actions_) {
    action->evaluate(t);
  }
}

// Normally, variables are inspected only once per rule, and only after all transformation functions
// have been completed. With multiMatch, variables are checked against the operator before and after
// every transformation function that changes the input.
inline bool Rule::evaluateWithMultiMatch(Transaction& t) const {
  // Get all of the transformations
  std::vector<Transformation::TransformBase*> transforms;
  if (!is_ingnore_default_transform_) {
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

  // Evaluate the variables
  bool rule_matched = false;
  for (auto& var : variables_) {
    Common::EvaluateResults result;
    evaluateVariable(t, var, result);

    size_t curr_transform_index = 0;

    // Evaluate each variable result
    for (size_t i = 0; i < result.size();) {
      Common::EvaluateResults::Element& variable_value = result.get(i);

      // Evaluate the operator
      bool variable_matched = evaluateOperator(t, variable_value.variant_);

      // If the variable is matched, evaluate the actions
      if (variable_matched) {
        t.pushMatchedVariable(var.get(), result.move(i));
        WGE_LOG_TRACE([&]() {
          if (!var->isCollection()) {
            return std::format("variable is matched. {}{}", var->mainName(),
                               var->subName().empty() ? "" : "." + var->subName());
          } else {
            auto& matched_var = t.getMatchedVariables().back();
            return std::format("variable of collection is matched. {}:{}", var->mainName(),
                               matched_var.second.variable_sub_name_);
          }
        }());

        rule_matched = true;

        // Evaluate the default actions and the action defined actions
        evaluateActions(t);

        // The variable value is matched, evaluate next variable value
        i++;
        curr_transform_index = 0;
      } else {
        // The variable value was moved by pushMatchedVariable, we need copy it back
        variable_value = t.getMatchedVariables().back().second;

        // The variable value is not matched, perf transformation and try to match again
        if (IS_STRING_VIEW_VARIANT(variable_value.variant_)) [[likely]] {
          // Evaluate the transformation
          bool ret = false;
          while (!ret && curr_transform_index < transforms.size()) {
            ret = transforms[curr_transform_index]->evaluate(t, var.get(), variable_value);
            WGE_LOG_TRACE("evaluate transformation: {} {}",
                          transforms[curr_transform_index]->name(), ret);
            curr_transform_index++;
          }

          // All of the transformations have been evaluated, and the variable value is not matched
          // We need to evaluate the next variable value
          if (!ret) {
            i++;
            curr_transform_index = 0;
          }
        } else {
          i++;
          curr_transform_index = 0;
        }
      }
    }
  }

  // Evaluate the chained rules
  if (rule_matched) {
    if (!chain_.empty()) [[unlikely]] {
      // If the chained rules are matched means the rule is matched, otherwise the rule is not
      // matched
      if (!evaluateChain(t)) {
        rule_matched = false;
      }
    }
  }

  // Evaluate the msg macro and logdata macro
  if (rule_matched) {
    evaluateMsgMacro(t);
    evaluateLogDataMacro(t);
  }

  return rule_matched;
}
} // namespace Wge