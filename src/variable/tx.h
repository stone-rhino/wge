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
  const Common::Variant& evaluate(Transaction& t) const override {
    if (matched_index_ == 0xffffffff) [[likely]] {
      if (is_counter_) {
        if (index_.has_value()) [[likely]] {
          return t.hasVariable(index_.value()) ? number_one_ : number_zero_;
        } else {
          return t.hasVariable(sub_name_) ? number_one_ : number_zero_;
        }
      } else {
        if (index_.has_value()) [[likely]] {
          return t.getVariable(index_.value());
        } else {
          return t.getVariable(sub_name_);
        }
      }
    } else {
      return t.getMatched(matched_index_);
    }
  }

private:
  static constexpr Common::Variant number_zero_{0};
  static constexpr Common::Variant number_one_{1};
  std::optional<size_t> index_;
  size_t matched_index_{0xffffffff};
};
} // namespace Variable
} // namespace SrSecurity