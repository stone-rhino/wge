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
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (!is_counter_) [[likely]] {
      result.set(t.httpExtractor().request_header_extractor_(sub_name_));
    } else {
      result.set(t.httpExtractor().request_header_extractor_(sub_name_).empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity