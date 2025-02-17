#include "rule.h"

#include "common/assert.h"
#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
bool Rule::evaluate(Transaction& t, const HttpExtractor& extractor) const {
  bool result = false;

  // Check whether the rule is unconditional(SecAction)
  bool is_uncondition = operator_ == nullptr;

  if (is_uncondition) [[unlikely]] {
    // Evaluate the actions
    for (auto& action : actions_) {
      action->evaluate(t);
    }
    result = true;
  } else [[likely]] {
    result = true;
    std::string transform_data;

    // Evaluate the variables
    for (auto& var : variables_) {
      auto var_value = var->evaluate(t);

      // Evaluate the transformations
      if (!is_ingnore_default_transform_) {
        auto& default_actions = t.getEngine().defaultActions(phase_);
        for (auto& action : default_actions) {
          for (auto& transform : action->transforms()) {
            transform_data = transform->evaluate(var_value.data(), var_value.size());
            var_value = transform_data;
          }
        }
      }
      for (auto& transform : transforms_) {
        transform_data = transform->evaluate(var_value.data(), var_value.size());
        var_value = transform_data;
      }

      // Evaluate the operator
      if (!operator_->evaluate(t, var_value)) {
        result = false;
        break;
      }
    }
  }

  return result;
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