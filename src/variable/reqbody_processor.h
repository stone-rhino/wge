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
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    auto body_processor_type = t.getRequestBodyProcessor();
    auto iter = body_processor_type_map_.find(body_processor_type);
    if (iter != body_processor_type_map_.end()) {
      if (!is_counter_) [[likely]] {
        result.set(iter->second);
      } else {
        result.set(1);
      }
    } else {
      if (!is_counter_) [[likely]] {
        result.clear();
      } else {
        result.set(0);
      }
    }
  };

private:
  static const std::unordered_map<BodyProcessorType, std::string_view> body_processor_type_map_;
};
} // namespace Variable
} // namespace SrSecurity