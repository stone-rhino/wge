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
/**
 * XML variable. Supports the following syntax:
 * - `XML://@*` for all attributes of all tags. When filling Common::EvaluateResults, each element
 * corresponds to the attribute value of a tag.
 * - `XML:/\*` for all tag values. When filling Common::EvaluateResults, there is only one element,
 * which concatenates all XML tag values into a string.
 * - `XML://@*@file@` for multi-pattern matching of attributes based on the specified file. When
 * filling Common::EvaluateResults, each element corresponds to the attribute value of a tag.
 * - `XML:/\*@file@` for multi-pattern matching of tag values based on the specified file. When
 * filling Common::EvaluateResults, each element corresponds to the value of a tag.
 */
class Xml final : public CollectionBase {
  DECLARE_VIRABLE_NAME(XML);

public:
  Xml(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : CollectionBase(
            [&]() -> std::string {
              std::string collection_sub_name;
              if (sub_name == "//@*") {
                type_ = Type::AttrValue;
              } else if (sub_name == "/*") {
                type_ = Type::TagValue;
              } else if (sub_name.ends_with("@")) {
                if (sub_name.starts_with("//@*@")) {
                  type_ = Type::AttrValuePmf;
                  collection_sub_name = sub_name.substr(4);
                } else if (sub_name.starts_with("/*@")) {
                  type_ = Type::TagValuePmf;
                  collection_sub_name = sub_name.substr(2);
                }
              }

              return collection_sub_name;
            }(),
            is_not, is_counter, curr_rule_file_path) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& kv_pairs = getKvPairs(t);

    result.emplace_back(static_cast<int64_t>(kv_pairs.size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollectionCounter(t, result);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto& kv_pairs = getKvPairs(t);

    switch (type_) {
    case Type::AttrValue: {
      for (auto& elem : kv_pairs) {
        result.emplace_back(elem.second);
      }
    } break;
    case Type::TagValue: {
      auto& tag_value_str = t.getBodyXml().getTagValuesStr();
      if (!tag_value_str.empty()) {
        result.emplace_back(tag_value_str);
      }
    } break;
    case Type::AttrValuePmf:
    case Type::TagValuePmf: {
      UNREACHABLE();
    } break;
    default:
      UNREACHABLE();
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    auto& kv_pairs = getKvPairs(t);

    if (!isRegex())
      [[likely]] {
        switch (type_) {
        case Type::AttrValue: {
          for (auto& elem : kv_pairs) {
            result.emplace_back(elem.second);
          }
        } break;
        case Type::TagValue: {
          auto& tag_value_str = t.getBodyXml().getTagValuesStr();
          if (!tag_value_str.empty()) {
            result.emplace_back(tag_value_str);
          }
        } break;
        case Type::AttrValuePmf:
        case Type::TagValuePmf: {
          UNREACHABLE();
        } break;
        default:
          UNREACHABLE();
        }
      }
    else {
      switch (type_) {
      case Type::AttrValue: {
        UNREACHABLE();
      } break;
      case Type::TagValue: {
        UNREACHABLE();
      } break;
      case Type::AttrValuePmf:
      case Type::TagValuePmf: {
        for (auto& elem : kv_pairs) {
          if (elem.second.empty()) {
            continue;
          }

          if (!hasExceptVariable(t, main_name_, elem.first))
            [[likely]] {
              if (match(elem.first)) {
                result.emplace_back(elem.second, elem.first);
              }
            }
        }
      } break;
      default:
        UNREACHABLE();
      }
    }
  }

private:
  const std::vector<std::pair<std::string_view, std::string_view>>&
  getKvPairs(Transaction& t) const {
    if (type_ == Type::TagValue || type_ == Type::TagValuePmf) {
      return t.getBodyXml().getTags();
    } else {
      return t.getBodyXml().getAttributes();
    }
  }

private:
  enum class Type { AttrValue, TagValue, AttrValuePmf, TagValuePmf };
  Type type_;
};
} // namespace Variable
} // namespace Wge