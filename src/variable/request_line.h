#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestLine : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_LINE);

public:
  RequestLine(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override {
    return t.getEvaluatedBuffer(Transaction::EvaluatedBufferType::Variable).set(t.getUri());
  };
};
} // namespace Variable
} // namespace SrSecurity