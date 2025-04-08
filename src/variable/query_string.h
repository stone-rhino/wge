#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class QueryString : public VariableBase {
  DECLARE_VIRABLE_NAME(QUERY_STRING);

public:
  QueryString(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (is_counter_) [[unlikely]] {
      result.append(t.getRequestLineInfo().query_.empty() ? 0 : 1);
      return;
    }

    result.append(t.getRequestLineInfo().query_);
  };
};
} // namespace Variable
} // namespace SrSecurity