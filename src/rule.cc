#include "rule.h"

#include <iostream>

#include "common/assert.h"
#include "common/log.h"
#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
bool Rule::evaluate(Transaction& t, const HttpExtractor& extractor) const {
  bool matched = false;

  // Check whether the rule is unconditional(SecAction)
  bool is_uncondition = operator_ == nullptr;

  if (is_uncondition) [[unlikely]] {
    // Evaluate the actions
    for (auto& action : actions_) {
      action->evaluate(t);
    }
    matched = true;
  } else [[likely]] {
    Common::Variant transform_data;

    // Evaluate the variables
    for (auto& var : variables_) {
      const Common::Variant* var_value = &var->evaluate(t);

      // Evaluate the default transformations
      if (!is_ingnore_default_transform_) [[unlikely]] {
        const SrSecurity::Rule* default_action = t.getEngine().defaultActions(phase_);
        if (default_action) {
          for (auto& transform : default_action->transforms()) {
            if (IS_STRING_VIEW_VARIANT(*var_value)) [[likely]] {
              const std::string_view& var_value_str = std::get<std::string_view>(*var_value);
              transform_data = transform->evaluate(var_value_str.data(), var_value_str.size());
              var_value = &transform_data;
            } else if (IS_STRING_VARIANT(*var_value)) [[unlikely]] {
              const std::string& var_value_str = std::get<std::string>(*var_value);
              transform_data = transform->evaluate(var_value_str.data(), var_value_str.size());
              var_value = &transform_data;
            } else {
              UNREACHABLE();
            }
          }
        }
      }

      // Evaluate the action defined transformations
      for (auto& transform : transforms_) {
        if (IS_STRING_VIEW_VARIANT(*var_value)) [[likely]] {
          const std::string_view& var_value_str = std::get<std::string_view>(*var_value);
          transform_data = transform->evaluate(var_value_str.data(), var_value_str.size());
          var_value = &transform_data;
        } else if (IS_STRING_VARIANT(*var_value)) [[unlikely]] {
          const std::string& var_value_str = std::get<std::string>(*var_value);
          transform_data = transform->evaluate(var_value_str.data(), var_value_str.size());
          var_value = &transform_data;
        } else {
          // UNREACHABLE();
          if (!var->subName().empty()) [[likely]] {
            SRSECURITY_LOG_WARN(
                "Rule try to transform a variant type that is not string. file: {}[{}] "
                "variable: {}.{} variant type: {}",
                file_path_, line_, var->mainName(), var->subName(), var_value->index());
          } else [[unlikely]] {
            SRSECURITY_LOG_WARN(
                "Rule try to transform a variant type that is not string. file: {}[{}] "
                "variable: {} variant type: {}",
                file_path_, line_, var->mainName(), var_value->index());
          }
        }
      }

      // Evaluate the operator
      matched = operator_->evaluate(t, *var_value);

      // Evaluate the chained rules
      if (matched && !chain_.empty()) [[unlikely]] {
        for (auto& rule : chain_) {
          matched = rule->evaluate(t, extractor);
          if (!matched) {
            break;
          }
        }

        // Don't evaluate the actions if the chained rules are not matched
        if (!matched) {
          break;
        }
      }

      // Evaluate the actions
      if (matched) {
        for (auto& action : actions_) {
          action->evaluate(t);
        }
        break;
      }
    }
  }

  return matched;
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

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) { operator_ = std::move(op); }

} // namespace SrSecurity