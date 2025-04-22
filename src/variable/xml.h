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
#include "evaluate_help.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class Xml : public VariableBase, public CollectionBase {
  DECLARE_VIRABLE_NAME(XML);

public:
  Xml(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter), CollectionBase(sub_name_) {
    if (sub_name_ == "//@*") {
      type_ = Type::AttrValue;
    }
  }

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    const std::vector<std::string_view>* values = nullptr;
    if (type_ == Type::AttrValue) {
      values = &(t.getBodyXml().getAttrValues());
    } else {
      values = &(t.getBodyXml().getTagValues());
    }

    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(values->size())); },
        // specify subname
        { result.append(static_cast<int>(values->size())); });

    RETURN_VALUE(
        // collection
        {
          for (auto& elem : *values) {
            result.append(elem);
          }
        },
        // collection regex
        {
          UNREACHABLE();
        },
        // specify subname
        {
          for (auto& elem : *values) {
            result.append(elem);
          }
        });
  };

  bool isCollection() const override { return sub_name_.empty(); };

private:
  enum class Type {
    AttrValue,
    TagValue,
  };

  Type type_{Type::TagValue};
};
} // namespace Variable
} // namespace Wge