#pragma once

#include <optional>

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Tx : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(TX);

public:
  Tx(std::string&& sub_name, std::optional<size_t> index, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), index_(index) {
    if (!sub_name_.empty() && std::all_of(sub_name_.begin(), sub_name_.end(), ::isdigit)) {
      capture_index_ = ::atoi(sub_name_.c_str());
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // Process capture that definded by TX:[1-99]
    if (capture_index_.has_value()) [[unlikely]] {
      if (!is_counter_) [[likely]] {
        evluateCapture(t, result);
      } else {
        evluateCaptureCount(t, result);
      }
      return;
    }

    // Process single variable and collection
    if (!is_counter_) [[likely]] {
      if (!sub_name_.empty()) [[likely]] {
        evluateVariable(t, result);
      } else {
        evluateCollection(t, result);
      }
    } else {
      if (!sub_name_.empty()) {
        evluateVariableCount(t, result);
      } else {
        evluateCollectionCount(t, result);
      }
    }
  }

  bool isCollection() const override { return sub_name_.empty(); };

private:
  void evluateCapture(Transaction& t, Common::EvaluateResults& result) const {
    result.append(t.getCapture(capture_index_.value()));
  }

  void evluateCaptureCount(Transaction& t, Common::EvaluateResults& result) const {
    result.append(IS_EMPTY_VARIANT(t.getCapture(capture_index_.value())) ? 1 : 0);
  }

  void evluateVariable(Transaction& t, Common::EvaluateResults& result) const {
    if (index_.has_value()) [[likely]] {
      result.append(t.getVariable(index_.value()));
    } else {
      result.append(t.getVariable(sub_name_));
    }
  }

  void evluateVariableCount(Transaction& t, Common::EvaluateResults& result) const {
    if (index_.has_value()) [[likely]] {
      t.hasVariable(index_.value()) ? result.append(1) : result.append(0);
    } else {
      t.hasVariable(sub_name_) ? result.append(1) : result.append(0);
    }
  }

  void evluateCollection(Transaction& t, Common::EvaluateResults& result) const {
    std::vector<std::pair<std::string_view, Common::Variant*>> variables = t.getVariables();
    for (auto variable : variables) {
      if (!hasExceptVariable(variable.first)) [[likely]] {
        result.append(*variable.second, variable.first);
      }
    }
  }

  void evluateCollectionCount(Transaction& t, Common::EvaluateResults& result) const {
    result.append(t.getVariablesCount());
  }

private:
  std::optional<size_t> index_;
  std::optional<size_t> capture_index_;
};
} // namespace Variable
} // namespace SrSecurity