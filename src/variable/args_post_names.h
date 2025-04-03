#pragma once

#include "collection_base.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsPostNames : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(ARGS_POST_NAMES);

public:
  ArgsPostNames(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    // Get the query params by the request body processor type
    const std::vector<std::unordered_map<std::string_view, std::string_view>::iterator>*
        query_params = nullptr;
    const std::unordered_map<std::string_view, std::string_view>* query_params_map = nullptr;
    switch (t.getRequestBodyProcessor()) {
    case BodyProcessorType::UrlEncoded:
      query_params = &t.getBodyQueryParam().getLinked();
      query_params_map = &t.getBodyQueryParam().get();
      break;
    case BodyProcessorType::MultiPart:
      query_params = &t.getBodyMultiPart().getLinked();
      query_params_map = &t.getBodyMultiPart().get();
      break;
    default:
      query_params = &t.getBodyQueryParam().getLinked();
      query_params_map = &t.getBodyQueryParam().get();
      break;
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(query_params->size())); },
        // specify subname
        {
          auto iter = query_params_map->find(sub_name_);
          result.append(iter != query_params_map->end() ? 1 : 0);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : *query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              result.append(elem->first);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : *query_params) {
            if (!hasExceptVariable(elem->first)) [[likely]] {
              if (match(elem->first)) {
                result.append(elem->first);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter = query_params_map->find(sub_name_);
            if (iter != query_params_map->end()) {
              result.append(iter->first);
            }
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity