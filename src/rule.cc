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

  SRSECURITY_LOG_TRACE("------------------------------------");
  if (is_uncondition) [[unlikely]] {
    SRSECURITY_LOG_TRACE("evaluate SecAction. id: {}", id_);
    // Evaluate the actions
    for (auto& action : actions_) {
      action->evaluate(t);
    }
    matched = true;

  } else [[likely]] {
    SRSECURITY_LOG_TRACE("evaluate SecRule. id: {}", id_);

    Common::Variant transform_data;

    // Evaluate the variables
    for (auto& var : variables_) {
      const Common::Variant* var_value = &var->evaluate(t);
      SRSECURITY_LOG_TRACE("evaluate variable: {}{}{}{} = {}", var->isNot() ? "!" : "",
                           var->isCounter() ? "&" : "", var->mainName(),
                           var->subName().empty() ? "" : "." + var->subName(),
                           VISTIT_VARIANT_AS_STRING(*var_value));

      // Evaluate the default transformations
      if (!is_ingnore_default_transform_) [[unlikely]] {
        const SrSecurity::Rule* default_action = t.getEngine().defaultActions(phase_);
        if (default_action) {
          for (auto& transform : default_action->transforms()) {
            SRSECURITY_LOG_TRACE("evaluate default transformation: {}", transform->name());
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
        SRSECURITY_LOG_TRACE("evaluate action defined transformation: {}", transform->name());
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
      SRSECURITY_LOG_TRACE("evaluate operator: {} {} {} = {}", VISTIT_VARIANT_AS_STRING(*var_value),
                           operator_->name(), operator_->literalValue(), matched);

      // Evaluate the chained rules
      if (matched && !chain_.empty()) [[unlikely]] {
        for (auto& rule : chain_) {
          SRSECURITY_LOG_TRACE("evaluate chained rule. id: {}", rule->id());
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

      // If the rule is matched, do some things such as macro expansion and evaluate the actions
      if (matched) {
        SRSECURITY_LOG_TRACE("Rule is matched. id: {}", id_);

        // Macro expansion
        if (msg_macro_) {
          Common::Variant& result = const_cast<Common::Variant&>(msg_macro_->evaluate(t));
          assert(IS_STRING_VARIANT(result));
          std::string& msg_macro_result = std::get<std::string>(result);
          t.evaluatedBuffer().msg_ = std::move(msg_macro_result);
          assert(std::get<std::string>(result).empty());
        }

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
        break;
      }
    } // end of for (auto& var : variables_)
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