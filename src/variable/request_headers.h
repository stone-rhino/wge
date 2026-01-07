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

#include "collection_base.h"

namespace Wge {
namespace Variable {
class RequestHeadersBase : public CollectionBase {
public:
  RequestHeadersBase(std::string&& sub_name, bool is_not, bool is_counter,
                     std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

  RequestHeadersBase(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not,
                     bool is_counter, std::string_view curr_rule_file_path)
      : CollectionBase(std::move(sub_name_macro), is_not, is_counter, curr_rule_file_path) {}

protected:
  virtual void addResultItem(Common::EvaluateResults& result, std::string_view key,
                             std::string_view value) const = 0;

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(static_cast<int64_t>(t.httpExtractor().request_header_count_));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    int64_t count = 0;
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : {
        count += t.httpExtractor().request_header_find_(sub_name_).size();
      }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto main_name = mainName();
      t.httpExtractor().request_header_traversal_(
          [&](std::string_view key, std::string_view value) {
            if (!hasExceptVariable(t, main_name, key))
              [[likely]] {
                if (match(key)) {
                  ++count;
                }
              }
            return true;
          });
    } break;
    case SubNameType::Macro: {
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          std::string_view sub_name = std::get<std::string_view>(r.variant_);
          std::vector<std::string_view> values =
              t.httpExtractor().request_header_find_(std::string(sub_name.data(), sub_name.size()));
          for (auto& value : values) {
            ++count;
          }
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
    auto main_name = mainName();
    t.httpExtractor().request_header_traversal_([&](std::string_view key, std::string_view value) {
      if (!hasExceptVariable(t, main_name, key))
        [[likely]] { addResultItem(result, key, value); }
      return true;
    });
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    switch (subNameType()) {
      [[likely]] case SubNameType::Literal : {
        std::vector<std::string_view> values = t.httpExtractor().request_header_find_(sub_name_);
        for (auto& value : values) {
          addResultItem(result, sub_name_, value);
        }
      }
      break;
    case SubNameType::Regex:
    case SubNameType::RegexFile: {
      auto main_name = mainName();
      t.httpExtractor().request_header_traversal_(
          [&](std::string_view key, std::string_view value) {
            if (!hasExceptVariable(t, main_name, key))
              [[likely]] {
                if (match(key)) {
                  addResultItem(result, key, value);
                }
              }
            return true;
          });
    } break;
    case SubNameType::Macro: {
      Common::EvaluateResults macro_result;
      evaluateMacro(t, macro_result);
      for (auto& r : macro_result) {
        assert(IS_STRING_VIEW_VARIANT(r.variant_));
        if (IS_STRING_VIEW_VARIANT(r.variant_)) {
          std::string_view sub_name = std::get<std::string_view>(r.variant_);
          std::vector<std::string_view> values =
              t.httpExtractor().request_header_find_(std::string(sub_name.data(), sub_name.size()));
          for (auto& value : values) {
            addResultItem(result, sub_name_, value);
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

class RequestHeaders final : public RequestHeadersBase {
  DECLARE_VIRABLE_NAME(REQUEST_HEADERS);

public:
  RequestHeaders(std::string&& sub_name, bool is_not, bool is_counter,
                 std::string_view curr_rule_file_path)
      : RequestHeadersBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path) {}

  RequestHeaders(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not,
                 bool is_counter, std::string_view curr_rule_file_path)
      : RequestHeadersBase(std::move(sub_name_macro), is_not, is_counter, curr_rule_file_path) {}

protected:
  void addResultItem(Common::EvaluateResults& result, std::string_view key,
                     std::string_view value) const override {
    result.emplace_back(value, key);
  }
};
} // namespace Variable
} // namespace Wge