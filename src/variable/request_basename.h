#pragma once

#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestBaseName : public VariableBase {
  DECLARE_VIRABLE_NAME(REQUEST_BASENAME);

public:
  RequestBaseName(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (!is_counter_) [[likely]] {
      auto pos = t.getRequestLineInfo().uri_.rfind('/');
      if (pos == std::string_view::npos) {
        result.append(t.getRequestLineInfo().uri_);
      } else {
        result.append(t.getRequestLineInfo().uri_.substr(pos + 1));
      }
    } else {
      result.append(t.getRequestLineInfo().uri_.empty() ? 0 : 1);
    }
  };
};
} // namespace Variable
} // namespace SrSecurity