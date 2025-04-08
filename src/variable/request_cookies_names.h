#pragma once

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestCookiesNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(REQUEST_COOKIES_NAMES);

public:
  RequestCookiesNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    const std::unordered_map<std::string_view, std::string_view>& cookies = t.getCookies();

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(cookies.size())); },
        // specify subname
        {
          auto iter = cookies.find(sub_name_);
          result.append(iter != cookies.end() ? 1 : 0);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : cookies) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              result.append(elem.first, elem.first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : cookies) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              if (match(elem.first)) {
                result.append(elem.first, elem.first);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter = cookies.find(sub_name_);
            if (iter != cookies.end()) {
              result.append(iter->first);
            }
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity