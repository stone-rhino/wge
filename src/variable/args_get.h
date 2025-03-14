#pragma once

#include <string>
#include <string_view>

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsGet : public VariableBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_GET);

public:
  ArgsGet(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResult& result) const override {
    if (!is_counter_) [[likely]] {
      for (auto& query_param : t.getRequestLineInfo().query_params_) {
        result.append(query_param.second);
      }
    } else {
      result.append(t.getRequestLineInfo().query_params_.empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity