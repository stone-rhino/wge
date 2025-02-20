#pragma once

#include "operator_base.h"

namespace SrSecurity {
namespace Operator {
class Le : public OperatorBase {
  DECLARE_OPERATOR_NAME(le);

public:
  Le(std::string&& literal_value) : OperatorBase(std::move(literal_value)) {
    if (!macro_) {
      value_ = ::atoll(literal_value_.c_str());
    }
  }

public:
  bool evaluate(Transaction& t, const Common::Variant& operand) const override {
    if (IS_EMPTY_VARIANT(operand)) [[unlikely]] {
      return false;
    }

    int64_t operand_value = std::get<int>(operand);
    if (macro_) {
      int64_t macro_value = std::get<int>(macro_->evaluate(t));
      return macro_value <= operand_value;
    } else {
      return value_ <= operand_value;
    }
  }

private:
  int64_t value_;
};
} // namespace Operator
} // namespace SrSecurity