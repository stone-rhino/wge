#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestBody : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_BODY);

public:
  RequestBody(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    const std::vector<std::string_view>& body = t.httpExtractor().reqeust_body_extractor_();
    for (const auto& elem : body) {
      result.append(elem);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity