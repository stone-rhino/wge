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

#include "persistent_collection_base.h"
#include "variable_base.h"

namespace Wge {
namespace Variable {
class Resource : public VariableBase, public PersistentCollectionBase {
  DECLARE_VIRABLE_NAME(RESOURCE);

public:
  Resource(std::string&& sub_name, bool is_not, bool is_counter)
      : VariableBase(std::move(sub_name), is_not, is_counter),
        PersistentCollectionBase(PersistentStorage::Storage::Type::RESOURCE, sub_name_) {}

public:
  void evaluate(Transaction& t, Common::EvaluateResults& result) const override {
    RETURN_IF_COUNTER(
        // collection
        { result.append(static_cast<int>(size(t))); },
        // specify subname
        {
          auto& value = get(t, sub_name_);
          result.append(IS_EMPTY_VARIANT(value) ? 0 : 1);
        });

    RETURN_VALUE(
        // collection
        {
          travel(t, [&](const std::string& key, const Common::Variant& value) {
            if (!hasExceptVariable(key)) [[likely]] {
              result.append(value, key);
            }
            return true;
          });
        },
        // collection regex
        {
          travel(t, [&](const std::string& key, const Common::Variant& value) {
            if (!hasExceptVariable(key)) [[likely]] {
              if (match(key)) {
                result.append(value, key);
              }
            }
            return true;
          });
        },
        // specify subname
        {
          auto& value = get(t, sub_name_);
          if (!IS_EMPTY_VARIANT(value)) [[likely]] {
            result.append(value);
          }
        });
  }

  bool isCollection() const override { return sub_name_.empty(); };
};
} // namespace Variable
} // namespace Wge