#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Session : public VariableBase {
  DECLARE_VIRABLE_NAME(SESSION);

public:
  Session(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override { assert(false); throw "Not implemented!"; };
};
} // namespace Variable
} // namespace SrSecurity