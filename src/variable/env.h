/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

#include "collection_base.h"

namespace Wge {
namespace Variable {
class Env final : public CollectionBase {
  DECLARE_VIRABLE_NAME(ENV);

public:
  Env(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

  Env(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
      std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name_macro), is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& env_map = t.getEnviron();
    result.emplace_back(static_cast<int64_t>(env_map.size()));
  }
  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    int64_t count = 0;
    auto& env_map = t.getEnviron();
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : { count = env_map.count(sub_name_); }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto main_name = mainName();
      for (auto& elem : env_map) {
        if (!hasExceptVariable(t, main_name, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              ++count;
            }
          }
      }
    } break;
    case SubNameType::Macro: {
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          count += env_map.count(std::get<std::string_view>(r.variant_));
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
    auto& env_map = t.getEnviron();
    auto main_name = mainName();
    for (auto& elem : env_map) {
      if (!hasExceptVariable(t, main_name, elem.first))
        [[likely]] { result.emplace_back(elem.second, elem.first); }
    }
  }
  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    auto& env_map = t.getEnviron();
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : {
        auto iter = env_map.find(sub_name_);
        if (iter != env_map.end()) {
          result.emplace_back(iter->second, iter->first);
        }
      }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto main_name = mainName();
      for (auto& elem : env_map) {
        if (!hasExceptVariable(t, main_name, elem.first))
          [[likely]] {
            if (match(elem.first)) {
              result.emplace_back(elem.second, elem.first);
            }
          }
      }
    } break;
    case SubNameType::Macro: {
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          auto iter = env_map.find(std::get<std::string_view>(r.variant_));
          if (iter != env_map.end()) {
            result.emplace_back(iter->second, iter->first);
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
} // namespace Variable
} // namespace Wge