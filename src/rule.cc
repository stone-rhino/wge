#include "rule.h"

#include "common/assert.h"
#include "common/try.h"

namespace SrSecurity {
bool Rule::evaluate(const HttpExtractor& extractor) const { return false; }

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  const std::string& name = var->fullName();
  auto iter = variables_map_.find(name);
  if (iter == variables_map_.end()) {
    var->preCompile();
    variables_pool_.emplace_back(std::move(var));
    variables_map_.insert({name, *variables_pool_.back()});
  }
}

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) {
  operator_ = std::move(op);
  operator_->preCompile();
}

} // namespace SrSecurity