#include "rule.h"

#include <iostream>

#include "common/assert.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"
#include "operator/rx.h"

namespace SrSecurity {
bool Rule::evaluate(Transaction& t) const {
  SRSECURITY_LOG_TRACE("------------------------------------");

  // Check whether the rule is unconditional(SecAction)
  bool is_uncondition = operator_ == nullptr;
  if (is_uncondition) [[unlikely]] {
    SRSECURITY_LOG_TRACE("evaluate SecAction. id: {} [{}:{}]", id_, file_path_, line_);
    // Evaluate the actions
    for (auto& action : actions_) {
      action->evaluate(t);
    }
    return true;
  }

  SRSECURITY_LOG_TRACE("evaluate SecRule. id: {} [{}:{}]", id_, file_path_, line_);

  // Evaluate the variables
  for (auto& var : variables_) {
    Common::EvaluateResult result;
    evaluateVariable(t, var, result);

    t.setCurrentVariable(var.get());
    // Evaluate each variable result
    for (size_t i = 0; i < result.size(); i++) {
      const Common::Variant& var_variant = result.get(i);
      t.setCurrentVariableResult(&var_variant);
      std::string_view var_str;
      bool matched = false;
      if (IS_STRING_VIEW_VARIANT(var_variant)) [[likely]] {
        var_str = std::get<std::string_view>(var_variant);

        // Evaluate the transformations
        std::string transforms_result;
        evaluateTransform(t, var_str, var, transforms_result);
        if (!transforms_result.empty()) {
          var_str = transforms_result;
        }

        // Evaluate the operator
        matched = evaluateOperator(t, var_str);
      } else {
        // Evaluate the operator
        matched = evaluateOperator(t, var_variant);
      }

      // If the rule is matched, do some things such as macro expansion and evaluate the actions
      if (matched) {
        SRSECURITY_LOG_TRACE("Rule is matched. id: {}", id_);

        // Macro expansion
        evaluateMsgMacro(t);
        evaluateLogDataMacro(t);

        // Evaluate the default actions and the action defined actions
        evaluateActions(t);

        // Evaluate the chained rules
        if (!chain_.empty()) [[unlikely]] {
          // If the chained rules are matched means the rule is matched, the remaining value of
          // variables or remaining variables are ignored.
          if (evaluateChain(t)) {
            return true;
          }
        } else {
          // Any variable is matched means the rule is matched
          return true;
        }
      }
    }
  }

  return false;
}

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  auto full_name = var->fullName();
  auto iter = variables_index_by_full_name_.find(full_name);
  if (iter == variables_index_by_full_name_.end()) {
    variables_.emplace_back(std::move(var));
    variables_index_by_full_name_.insert({full_name, *variables_.back()});
  }
}

void Rule::removeVariable(const Variable::VariableBase::FullName& full_name) {
  auto iter = variables_index_by_full_name_.find(full_name);
  if (iter != variables_index_by_full_name_.end()) {
    variables_index_by_full_name_.erase(iter);
    std::erase_if(variables_, [&](const std::unique_ptr<Variable::VariableBase>& var) {
      if (var->fullName() == full_name) {
        return true;
      }
      return false;
    });
  }
}

void Rule::capture(bool value) {
  Operator::Rx* rx = dynamic_cast<Operator::Rx*>(operator_.get());
  if (rx) {
    rx->capture(value);
  }
  capture_ = value;
}

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) { operator_ = std::move(op); }

inline void Rule::evaluateVariable(Transaction& t,
                                   const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                                   Common::EvaluateResult& result) const {
  var->evaluate(t, result);
  const Common::Variant* var_value = &result.front();
  SRSECURITY_LOG_TRACE("evaluate variable: {}{}{}{} = {}", var->isNot() ? "!" : "",
                       var->isCounter() ? "&" : "", var->mainName(),
                       var->subName().empty() ? "" : "." + var->subName(),
                       VISTIT_VARIANT_AS_STRING(*var_value));
}

inline void Rule::evaluateTransform(Transaction& t, std::string_view var_value,
                                    const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                                    std::string& result) const {
  // Check if the default transformation should be ignored
  if (is_ingnore_default_transform_) [[likely]] {
    return;
  }

  // Check that the default action is defined
  const SrSecurity::Rule* default_action = t.getEngine().defaultActions(phase_);
  if (!default_action) [[likely]] {
    return;
  }

  // Get the default transformation
  auto& transforms = default_action->transforms();
  if (transforms.empty()) [[likely]] {
    return;
  }

  // Evaluate the default transformations
  for (auto& t : transforms) {
    SRSECURITY_LOG_TRACE("evaluate default transformation: {}", t->name());
    result = t->evaluate(var_value.data(), var_value.size());
    var_value = result;
  }

  // Evaluate the action defined transformations
  for (auto& transform : transforms_) {
    SRSECURITY_LOG_TRACE("evaluate action defined transformation: {}", transform->name());
    result = transform->evaluate(var_value.data(), var_value.size());
    var_value = result;
  }
}

inline bool Rule::evaluateOperator(Transaction& t, const Common::Variant& var_value) const {
  bool matched = operator_->evaluate(t, var_value);
  SRSECURITY_LOG_TRACE(
      "evaluate operator: {} {}@{} {} = {}", VISTIT_VARIANT_AS_STRING(var_value),
      operator_->isNot() ? "!" : "", operator_->name(),
      operator_->macro() ? operator_->macro()->literalValue() : operator_->literalValue(), matched);
  return matched;
}

inline bool Rule::evaluateChain(Transaction& t) const {
  bool matched = true;
  for (auto& rule : chain_) {
    SRSECURITY_LOG_TRACE("evaluate chained rule. id: {}", rule->id());
    SRSECURITY_LOG_TRACE("↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
    matched = rule->evaluate(t);
    if (!matched) {
      break;
    }
  }

  return matched;
}

inline void Rule::evaluateMsgMacro(Transaction& t) const {
  if (msg_macro_) [[unlikely]] {
    Common::EvaluateResult msg_result;
    msg_macro_->evaluate(t, msg_result);
    t.setMsgMacroExpanded(msg_result.moveString(0));
    SRSECURITY_LOG_TRACE("evaluate msg macro: {}", t.getMsgMacroExpanded());
  }
}

inline void Rule::evaluateLogDataMacro(Transaction& t) const {
  if (log_data_macro_) [[unlikely]] {
    Common::EvaluateResult log_data_result;
    log_data_macro_->evaluate(t, log_data_result);
    t.setLogDataMacroExpanded(log_data_result.moveString(0));
    SRSECURITY_LOG_TRACE("evaluate logdata macro: {}", t.getLogDataMacroExpanded());
  }
}

inline void Rule::evaluateActions(Transaction& t) const {
  // Evaluate the default actions
  const SrSecurity::Rule* default_action = t.getEngine().defaultActions(phase_);
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
} // namespace SrSecurity