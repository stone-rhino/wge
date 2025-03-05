#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class MultipartBoundaryWhitespace : public VariableBase {
  DECLARE_VIRABLE_NAME(MULTIPART_BOUNDARY_WHITESPACE);

public:
  MultipartBoundaryWhitespace(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override {
    UNREACHABLE();
    throw "Not implemented!";
  };
};
} // namespace Variable
} // namespace SrSecurity