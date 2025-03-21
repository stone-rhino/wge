#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestHeadersNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(REQUEST_HEADERS_NAMES);

public:
  RequestHeadersNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    if (!is_counter_) [[likely]] {
      if (sub_name_.empty()) {
        t.httpExtractor().request_header_traversal_(
            [&](std::string_view key, std::string_view value) {
              if (!hasExceptVariable(key)) [[likely]] {
                result.append(key);
              }
              return true;
            });
      } else {
        std::string_view value = t.httpExtractor().request_header_find_(sub_name_);
        if (!value.empty()) {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            result.append(sub_name_);
          }
        }
      }
    } else {
      result.append(t.httpExtractor().request_header_count_ ? 1 : 0);
    }
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity