#pragma once

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class RequestHeadersNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(REQUEST_HEADERS_NAMES);

public:
  RequestHeadersNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(t.httpExtractor().request_header_count_)); },
        // specify subname
        { result.append(t.httpExtractor().request_header_find_(sub_name_).empty() ? 0 : 1); });

    RETURN_VALUE(
        // collection
        {
          t.httpExtractor().request_header_traversal_(
              [&](std::string_view key, std::string_view value) {
                if (!hasExceptVariable(key)) [[likely]] {
                  result.append(key);
                }
                return true;
              });
        },
        // collection regex
        {
          t.httpExtractor().request_header_traversal_(
              [&](std::string_view key, std::string_view value) {
                if (!hasExceptVariable(key)) [[likely]] {
                  if (match(key)) {
                    result.append(key);
                  }
                }
                return true;
              });
        },
        // specify subname
        {
          std::string_view value = t.httpExtractor().request_header_find_(sub_name_);
          if (!value.empty()) {
            if (!hasExceptVariable(sub_name_)) [[likely]] {
              result.append(sub_name_);
            }
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity