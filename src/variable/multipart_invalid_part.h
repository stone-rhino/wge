#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MultipartInvalidPart : public VariableBase {
  DECLARE_VIRABLE_NAME(MULTIPART_INVALID_PART);

public:
  MultipartInvalidPart(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    UNREACHABLE();
    throw "Not implemented!";
  };
};
} // namespace Variable
} // namespace SrSecurity