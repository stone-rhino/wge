/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <string_view>

#include "collection_base.h"

namespace Wge {
namespace Variable {
class ArgsGetBase : public CollectionBase {
public:
  ArgsGetBase(std::string&& sub_name, bool is_not, bool is_counter,
              std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

  ArgsGetBase(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
              std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name_macro), is_not, is_counter, curr_rule_file_path) {}

protected:
  virtual void addResultItem(Common::EvaluateResults& result, std::string_view key,
                             std::string_view value) const = 0;

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& query_params = t.getRequestLineInfo().query_params_.getLinked();

    result.emplace_back(static_cast<int64_t>(query_params.size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    int64_t count = 0;
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : {
        auto& query_params_map = t.getRequestLineInfo().query_params_.get();
        count = query_params_map.count(sub_name_);
      }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto& query_params = t.getRequestLineInfo().query_params_.getLinked();
      int64_t count = 0;
      auto main_name = mainName();
      for (auto& elem : query_params) {
        if (!hasExceptVariable(t, main_name, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              ++count;
            }
          }
      }
    } break;
    case SubNameType::Macro: {
      auto& query_params_map = t.getRequestLineInfo().query_params_.get();
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          count += query_params_map.count(std::get<std::string_view>(r.variant_));
        }
      }
    } break;
    default:
      UNREACHABLE();
      break;
    }

    result.emplace_back(count);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto& query_params = t.getRequestLineInfo().query_params_.getLinked();
    auto main_name = mainName();
    for (auto& elem : query_params) {
      if (!hasExceptVariable(t, main_name, elem.first))
        [[likely]] { addResultItem(result, elem.first, elem.second); }
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : {
        auto& query_params_map = t.getRequestLineInfo().query_params_.get();
        auto range = query_params_map.equal_range(sub_name_);
        for (auto iter = range.first; iter != range.second; ++iter) {
          addResultItem(result, iter->first, iter->second);
        }
      }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto& query_params = t.getRequestLineInfo().query_params_.getLinked();
      auto main_name = mainName();
      for (auto& elem : query_params) {
        if (!hasExceptVariable(t, main_name, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              addResultItem(result, elem.first, elem.second);
            }
          }
      }
    } break;
    case SubNameType::Macro: {
      auto& query_params_map = t.getRequestLineInfo().query_params_.get();
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          auto range = query_params_map.equal_range(std::get<std::string_view>(r.variant_));
          for (auto iter = range.first; iter != range.second; ++iter) {
            addResultItem(result, iter->first, iter->second);
          }
        }
      }
    } break;
    default:
      UNREACHABLE();
      break;
    }
  }
};

class ArgsGet final : public ArgsGetBase {
  friend class Args;
  DECLARE_VIRABLE_NAME(ARGS_GET);

public:
  ArgsGet(std::string&& sub_name, bool is_not, bool is_counter,
          std::string_view curr_rule_file_path)
      : ArgsGetBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

  ArgsGet(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
          std::string_view curr_rule_file_path)
      : ArgsGetBase(std::move(sub_name_macro), is_not, is_counter, curr_rule_file_path) {}

protected:
  void addResultItem(Common::EvaluateResults& result, std::string_view key,
                     std::string_view value) const override {
    result.emplace_back(value, key);
  }
};
} // namespace Variable
} // namespace Wge