#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MultipartStrictError : public VariableBase {
  DECLARE_VIRABLE_NAME(MULTIPART_STRICT_ERROR);

public:
  MultipartStrictError(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override { assert(false); throw "Not implemented!"; };
};
} // namespace Variable
} // namespace SrSecurity