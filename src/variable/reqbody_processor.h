#pragma once

#include <unordered_map>

#include "variable_base.h"

#include "../config.h"

namespace SrSecurity {
namespace Variable {
class ReqBodyProcessor : public VariableBase {
  DECLARE_VIRABLE_NAME(REQBODY_PROCESSOR);

public:
  ReqBodyProcessor(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  const Common::Variant& evaluate(Transaction& t) const override {
    auto body_processor_type = t.getRequestBodyProcessor();
    auto iter = body_processor_type_map_.find(body_processor_type);
    auto& buffer = t.evaluatedBuffer().variable_;
    if (iter != body_processor_type_map_.end()) {
      buffer = iter->second;
    } else {
      buffer = EMPTY_VARIANT;
    }
    return buffer;
  };

private:
  static const std::unordered_map<BodyProcessorType, std::string_view> body_processor_type_map_;
};
} // namespace Variable
} // namespace SrSecurity