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
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    result.append(t.getBodyMultiPart().getError().get(
                      SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError)
                      ? 1
                      : 0);
  };
};
} // namespace Variable
} // namespace SrSecurity