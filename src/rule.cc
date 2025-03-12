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
    Common::EvaluateResult variable_result;
    evaluateVariable(t, var, variable_result);

    // Evaluate each variable result
    for (size_t i = 0; i < variable_result.size(); i++) {
      const Common::Variant* flow_variant = &variable_result.get(i);

      // Evaluate the default transformations
      Common::EvaluateResult default_transform_result;
      bool ret = evaluateDefalutTransform(t, *flow_variant, var, default_transform_result);
      if (ret) [[unlikely]] {
        flow_variant = &default_transform_result.front();
      }

      // Evaluate the action defined transformations
      Common::EvaluateResult action_transform_result;
      ret = evaluateActionTransform(t, *flow_variant, var, action_transform_result);
      if (ret) [[unlikely]] {
        flow_variant = &action_transform_result.front();
      }

      // Evaluate the operator
      bool matched = evaluateOperator(t, *flow_variant);

      // Evaluate the chained rules
      if (matched && !chain_.empty()) [[unlikely]] {
        // If the chained rules are not matched means the rule is not matched
        if (!evaluateChain(t)) {
          return false;
        }
      }

      // FIXME(zhouyu 2025-03-13): Ensure that the chained rule can't be evaluated the actions.
      // May be we can add a flag to implement this feature.

      // If the rule is matched, do some things such as macro expansion and evaluate the actions
      if (matched) {
        SRSECURITY_LOG_TRACE("Rule is matched. id: {}", id_);

        // Macro expansion
        evaluateMsgMacro(t);
        evaluateLogDataMacro(t);

        // Evaluate the default actions and the action defined actions
        evaluateActions(t);

        // Any variable is matched means the rule is matched
        return true;
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

inline bool
Rule::evaluateDefalutTransform(Transaction& t, const Common::Variant& var_value,
                               const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                               Common::EvaluateResult& result) const {
  // Check if the default transformation should be ignored
  if (is_ingnore_default_transform_) [[likely]] {
    return false;
  }

  // Check that the default action is defined
  const SrSecurity::Rule* default_action = t.getEngine().defaultActions(phase_);
  if (!default_action) [[likely]] {
    return false;
  }

  // Get the default transformation
  auto& transforms = default_action->transforms();
  if (transforms.empty()) [[likely]] {
    return false;
  }

  // Only string type can be transformed
  if (!IS_STRING_VIEW_VARIANT(var_value)) [[unlikely]] {
    UNREACHABLE();
    if (!var->subName().empty()) [[likely]] {
      SRSECURITY_LOG_WARN("Rule try to transform a variant type that is not string. file: {}[{}] "
                          "variable: {}.{} variant type: {}",
                          file_path_, line_, var->mainName(), var->subName(), var_value.index());
    } else [[unlikely]] {
      SRSECURITY_LOG_WARN("Rule try to transform a variant type that is not string. file: {}[{}] "
                          "variable: {} variant type: {}",
                          file_path_, line_, var->mainName(), var_value.index());
    }
    return false;
  }

  std::string buffer;
  std::string_view data = std::get<std::string_view>(var_value);
  for (auto& t : transforms) {
    SRSECURITY_LOG_TRACE("evaluate default transformation: {}", t->name());
    buffer = t->evaluate(data.data(), data.size());
    data = buffer;
  }

  if (!buffer.empty()) [[likely]] {
    result.append(std::move(buffer));
    return true;
  }

  return false;
}

inline bool
Rule::evaluateActionTransform(Transaction& t, const Common::Variant& var_value,
                              const std::unique_ptr<SrSecurity::Variable::VariableBase>& var,
                              Common::EvaluateResult& result) const {
  if (transforms_.empty()) [[likely]] {
    return false;
  }

  // Only string type can be transformed
  if (!IS_STRING_VIEW_VARIANT(var_value)) [[unlikely]] {
    // UNREACHABLE();
    if (!var->subName().empty()) [[likely]] {
      SRSECURITY_LOG_WARN("Rule try to transform a variant type that is not string. file: {}[{}] "
                          "variable: {}.{} variant type: {}",
                          file_path_, line_, var->mainName(), var->subName(), var_value.index());
    } else [[unlikely]] {
      SRSECURITY_LOG_WARN("Rule try to transform a variant type that is not string. file: {}[{}] "
                          "variable: {} variant type: {}",
                          file_path_, line_, var->mainName(), var_value.index());
    }
    return false;
  }

  std::string buffer;
  std::string_view data = std::get<std::string_view>(var_value);
  for (auto& transform : transforms_) {
    SRSECURITY_LOG_TRACE("evaluate action defined transformation: {}", transform->name());
    buffer = transform->evaluate(data.data(), data.size());
    data = buffer;
  }

  if (!buffer.empty()) [[likely]] {
    result.append(std::move(buffer));
    return true;
  }

  return false;
}

inline bool Rule::evaluateOperator(Transaction& t, const Common::Variant& var_value) const {
  bool matched = operator_->evaluate(t, var_value);
  SRSECURITY_LOG_TRACE(
      "evaluate operator: {} {} {} = {}", VISTIT_VARIANT_AS_STRING(var_value), operator_->name(),
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