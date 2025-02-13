#include "rule.h"

#include "common/assert.h"
#include "common/likely.h"
#include "common/try.h"

namespace SrSecurity {
bool Rule::evaluate(const HttpExtractor& extractor, Transaction& t) const {
  // Check whether the rule is unconditional(SecAction)
  bool is_uncondition = operator_ == nullptr;

  if (unlikely(is_uncondition)) {
    for (auto& action : actions_) {
      action->evaluate(t);
    }
  } else {
  }

  return false;
}

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  auto full_name = var->fullName();
  auto iter = variables_index_by_full_name_.find(full_name);
  if (iter == variables_index_by_full_name_.end()) {
    var->preCompile();
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

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) {
  operator_ = std::move(op);
  operator_->preCompile();
}

} // namespace SrSecurity