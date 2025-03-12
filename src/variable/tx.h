#pragma once

#include <optional>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Tx : public VariableBase {
  DECLARE_VIRABLE_NAME(TX);

public:
  Tx(std::string&& sub_name, std::optional<size_t> index, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), index_(index) {
    if (std::all_of(sub_name_.begin(), sub_name_.end(), ::isdigit)) {
      matched_index_ = ::atoi(sub_name_.c_str());
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (matched_index_ == 0xffffffff) [[likely]] {
      if (is_counter_) {
        if (index_.has_value()) [[likely]] {
          t.hasVariable(index_.value()) ? result.append(1) : result.append(0);
        } else {
          t.hasVariable(sub_name_) ? result.append(1) : result.append(0);
        }
      } else {
        if (index_.has_value()) [[likely]] {
          result.append(t.getVariable(index_.value()));
        } else {
          result.append(t.getVariable(sub_name_));
        }
      }
    } else {
      result.append(t.getMatched(matched_index_));
    }
  }

private:
  std::optional<size_t> index_;
  size_t matched_index_{0xffffffff};
};
} // namespace Variable
} // namespace SrSecurity