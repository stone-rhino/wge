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

#include "variable_base.h"

namespace Wge {
namespace Variable {
class AuthType final : public VariableBase {
  DECLARE_VIRABLE_NAME(AUTH_TYPE);

public:
  AuthType(std::string&& sub_name, bool is_not, bool is_counter,
           std::string_view curr_rule_file_path)
      : VariableBase(std::move(sub_name), is_not, is_counter) {}

  AuthType(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not, bool is_counter,
           std::string_view curr_rule_file_path)
      : VariableBase("", is_not, is_counter) {
    // Does not support sub_name macro
    UNREACHABLE();
  }

protected:
  void evaluateCollectionCounter(Transaction& t, Common::EvaluateResults& result) const override {
    result.emplace_back(
        static_cast<int64_t>(t.httpExtractor().request_header_find_("authorization").size()));
  }

  void evaluateSpecifyCounter(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollectionCounter(t, result);
  }

  void evaluateCollection(Transaction& t, Common::EvaluateResults& result) const override {
    auto auth_headers = t.httpExtractor().request_header_find_("authorization");
    for (auto auth_type : auth_headers) {
      auto pos = auth_type.find(' ');
      if (pos != std::string_view::npos) {
        auth_type = auth_type.substr(0, pos);
      }
      result.emplace_back(auth_type);
    }
  }

  void evaluateSpecify(Transaction& t, Common::EvaluateResults& result) const override {
    evaluateCollection(t, result);
  }
};
} // namespace Variable
} // namespace Wge