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
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto body_processor_type = t.getRequestBodyProcessor();
    auto iter = body_processor_type_map_.find(body_processor_type);

    if (is_counter_) [[unlikely]] {
      if (iter != body_processor_type_map_.end()) {
        result.append(1);
      } else {
        result.append(0);
      }
      return;
    }

    if (iter == body_processor_type_map_.end()) [[unlikely]] {
      return;
    }

    result.append(iter->second);
  };

private:
  static const std::unordered_map<BodyProcessorType, std::string_view> body_processor_type_map_;
};
} // namespace Variable
} // namespace SrSecurity