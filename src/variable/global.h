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

namespace Wge {
namespace Variable {
class Global final : public PersistentCollectionBase {
  DECLARE_VIRABLE_NAME(GLOBAL);

public:
  Global(std::string&& sub_name, bool is_not, bool is_counter, std::string_view curr_rule_file_path)
      : PersistentCollectionBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path,
                                 PersistentStorage::Storage::Type::GLOBAL) {}

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(static_cast<int64_t>(size(t)));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    auto& value = get(t, sub_name_);
    result.emplace_back(IS_EMPTY_VARIANT(value) ? 0 : 1);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    travel(t, [&](const std::string& key, const Common::Variant& value) {
      if (!hasExceptVariable(t, main_name_, key))
        [[likely]] { result.emplace_back(value, key); }
      return true;
    });
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    if (!isRegex())
      [[likely]] {
        auto& value = get(t, sub_name_);
        if (!IS_EMPTY_VARIANT(value))
          [[likely]] { result.emplace_back(value); }
      }
    else {
      travel(t, [&](const std::string& key, const Common::Variant& value) {
        if (!hasExceptVariable(t, main_name_, key))
          [[likely]] {
            if (match(key)) {
              result.emplace_back(value, key);
            }
          }
        return true;
      });
    }
  }
};
} // namespace Variable
} // namespace Wge