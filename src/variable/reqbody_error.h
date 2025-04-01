#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ReqBodyError : public VariableBase {
  DECLARE_VIRABLE_NAME(REQBODY_ERROR);

public:
  ReqBodyError(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    result.append(t.getReqBodyErrorMsg().empty() ? 0 : 1);
  };
};
} // namespace Variable
} // namespace SrSecurity