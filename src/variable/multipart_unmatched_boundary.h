#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MultipartUnmatchedBoundary : public VariableBase {
  DECLARE_VIRABLE_NAME(MULTIPART_UNMATCHED_BOUNDARY);

public:
  MultipartUnmatchedBoundary(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    result.append(t.getBodyMultiPart().getError().get(
                      SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary)
                      ? 1
                      : 0);
  };
};
} // namespace Variable
} // namespace SrSecurity