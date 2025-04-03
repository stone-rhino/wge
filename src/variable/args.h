#pragma once

#include <string>
#include <string_view>

#include "args_get.h"
#include "args_post.h"
#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class Args : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS);

public:
  Args(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto& line_query_params = t.getRequestLineInfo().query_params_.getLinked();
    auto& line_query_params_map = t.getRequestLineInfo().query_params_.get();

    // Get the query params by the request body processor type
    const std::vector<std::unordered_map<std::string_view, std::string_view>::iterator>*
        body_query_params = nullptr;
    const std::unordered_map<std::string_view, std::string_view>* body_query_params_map = nullptr;
    switch (t.getRequestBodyProcessor()) {
    case BodyProcessorType::UrlEncoded:
      body_query_params = &t.getBodyQueryParam().getLinked();
      body_query_params_map = &t.getBodyQueryParam().get();
      break;
    case BodyProcessorType::MultiPart:
      body_query_params = &t.getBodyMultiPart().getLinked();
      body_query_params_map = &t.getBodyMultiPart().get();
      break;
    default:
      body_query_params = &t.getBodyQueryParam().getLinked();
      body_query_params_map = &t.getBodyQueryParam().get();
      break;
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(line_query_params.size() + body_query_params->size())); },
        // specify subname
        {
          int count = line_query_params_map.find(sub_name_) != line_query_params_map.end() ? 1 : 0;
          if (body_query_params_map->find(sub_name_) != body_query_params_map->end()) {
            count++;
          }
          result.append(count);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : line_query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              result.append(elem->second);
            }
          }
          for (auto& elem : *body_query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              result.append(elem->second);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : line_query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              if (match(elem->first)) {
                result.append(elem->second);
              }
            }
          }
          for (auto& elem : *body_query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              if (match(elem->first)) {
                result.append(elem->second);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter = line_query_params_map.find(sub_name_);
            if (iter != line_query_params_map.end()) {
              result.append(iter->second);
            }
            auto iter2 = body_query_params_map->find(sub_name_);
            if (iter2 != body_query_params_map->end()) {
              result.append(iter2->second);
            }
          }
        });
  };
  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity