#pragma once

#include <string>
#include <string_view>

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsGet : public VariableBase, public CollectionBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_GET);

public:
  ArgsGet(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (!is_counter_) [[likely]] {
      for (auto& query_param : t.getRequestLineInfo().query_params_) {
        if (!hasExceptVariable(query_param.first)) [[likely]] {
          result.append(query_param.second);
        }
      }
    } else {
      result.append(t.getRequestLineInfo().query_params_.empty() ? 0 : 1);
    }
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity