#pragma once

#include <string>
#include <string_view>

#include "collection_base.h"
#include "evaluate_help.h"
#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
class ArgsGet : public VariableBase, public CollectionBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_GET);

public:
  ArgsGet(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    auto& query_params = t.getRequestLineInfo().query_params_;
    auto& query_params_map = t.getRequestLineInfo().query_params_map_;

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(query_params.size())); },
        // specify subname
        {
          auto iter = query_params_map.find(sub_name_);
          result.append(iter != query_params_map.end() ? 1 : 0);
        });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              result.append(elem.second);
            }
          }
        },
        // collection regex
        {
          for (auto& elem : query_params) {
            if (!hasExceptVariable(elem.first)) [[likely]] {
              if (match(elem.first)) {
                result.append(elem.second);
              }
            }
          }
        },
        // specify subname
        {
          if (!hasExceptVariable(sub_name_)) [[likely]] {
            auto iter = query_params_map.find(sub_name_);
            if (iter != query_params_map.end()) {
              result.append(iter->second);
            }
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace SrSecurity