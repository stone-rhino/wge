#pragma once

#include "collection_base.h"
#include "request_headers.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestCookies : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(REQUEST_COOKIES);

public:
  RequestCookies(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_map<std::string_view, std::string_view>& cookies = t.getCookies();
    if (cookies.empty()) [[unlikely]] {
      return;
    }

    if (!is_counter_) [[likely]] {
      if (sub_name_.empty()) {
        for (const auto& [key, value] : cookies) {
          if (!hasExceptVariable(key)) [[likely]] {
            result.append(value);
          }
        }
      } else {
        auto iter = cookies.find(sub_name_);
        if (iter != cookies.end()) {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            result.append(iter->second);
          }
        }
      }
    } else {
      result.append(static_cast<int>(cookies.size()));
    }
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity