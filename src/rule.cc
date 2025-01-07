#include "rule.h"

#include "common/assert.h"
#include "common/try.h"

namespace SrSecurity {
bool Rule::evaluate(const HttpExtractor& extractor) const { return false; }

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  const std::string& name = var->fullName();
  auto iter = variables_index_by_full_name_.find(name);
  if (iter == variables_index_by_full_name_.end()) {
    var->preCompile();
    variables_pool_.emplace_back(std::move(var));
    variables_index_by_full_name_.insert({name, *variables_pool_.back()});
  }
}

void Rule::removeVariable(const std::string& full_name) {
  auto iter = variables_index_by_full_name_.find(full_name);
  if (iter != variables_index_by_full_name_.end()) {
    variables_index_by_full_name_.erase(iter);
    std::erase_if(variables_pool_, [&](const std::unique_ptr<Variable::VariableBase>& var) {
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