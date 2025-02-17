#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MatchedVar : public VariableBase {
  DECLARE_VIRABLE_NAME(MATCHED_VAR);

public:
  MatchedVar(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  std::string_view evaluate(Transaction& t)  const override { assert(false); throw "Not implemented!"; };
};
} // namespace Variable
} // namespace SrSecurity