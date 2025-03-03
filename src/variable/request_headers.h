#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestHeaders : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_HEADERS);

public:
  RequestHeaders(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override {
    auto& buffer = t.evaluatedBuffer().variable_;
    buffer = t.httpExtractor().request_header_extractor_(sub_name_);
    return buffer;
  };
};
} // namespace Variable
} // namespace SrSecurity