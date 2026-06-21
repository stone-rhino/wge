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

#include "multipart_strict_error.h"

namespace Wge {
namespace Variable {
class MultipartBoundaryQuoted final : public MultipartErrorBase {
  DECLARE_VIRABLE_NAME(MULTIPART_BOUNDARY_QUOTED);

public:
  MultipartBoundaryQuoted(std::string&& sub_name, bool is_not, bool is_counter,
                          std::string_view curr_rule_file_path)
      : MultipartErrorBase(std::move(sub_name), is_not, is_counter, curr_rule_file_path,
                           Wge::MultipartStrictError::ErrorType::BoundaryQuoted) {}

  MultipartBoundaryQuoted(std::unique_ptr<Macro::VariableMacro>&& sub_name_macro, bool is_not,
                          bool is_counter, std::string_view curr_rule_file_path)
      : MultipartErrorBase("", is_not, is_counter, curr_rule_file_path,
                           Wge::MultipartStrictError::ErrorType::BoundaryQuoted) {
    // Does not support sub_name macro
    UNREACHABLE();
  }
};
} // namespace Variable
} // namespace Wge