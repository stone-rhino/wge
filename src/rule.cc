#include "rule.h"

#include "common/assert.h"

namespace SrSecurity {
bool Rule::evaluate(const HttpExtractor& extractor) const { return false; }

void Rule::preEvaluate() {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  // Save the id from actions to get id faster
  {
    static const std::string id_key = "id";
    auto iter = actions_map_.find(id_key);
    if (iter != actions_map_.end()) {
      try {
        id_ = std::stoull(iter->second.value());
      } catch (...) {
      }
    }
  }

  // Save the phase from actions to get phase faster
  {
    static const std::string phase_key = "phase";
    auto iter = actions_map_.find(phase_key);
    if (iter != actions_map_.end()) {
      try {
        phase_ = std::stoi(iter->second.value());
      } catch (...) {
      }
    }
  }

  initHyperscan();
  initPcre();
}

bool Rule::hasTag(const std::string& tag) const {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return true;
}

bool Rule::hasTag(const std::unordered_set<std::string>& tags) const {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  for (auto& tag : tags) {
    if (hasTag(tag)) {
      return true;
    }
  }

  return false;
}

void Rule::appendVariable(std::unique_ptr<Variable::VariableBase>&& var) {
  const std::string& name = var->fullName();
  auto iter = variables_map_.find(name);
  if (iter == variables_map_.end()) {
    var->preCompile();
    variables_pool_.emplace_back(std::move(var));
    variables_map_.insert({name, *variables_pool_.back()});
  }
}

void Rule::setOperator(std::unique_ptr<Operator::OperatorBase>&& op) { operator_ = std::move(op); }

void Rule::appendAction(std::unique_ptr<Action::ActionBase>&& action) {
  const std::string& name = action->name();
  auto iter = actions_map_.find(name);
  if (iter != actions_map_.end()) {
    actions_pool_.emplace_back(std::move(action));
    actions_map_.insert({name, *actions_pool_.back()});
  }
}
} // namespace SrSecurity