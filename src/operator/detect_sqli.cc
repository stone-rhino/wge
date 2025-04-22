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
#include "detect_sqli.h"

#include <libinjection.h>

#include "../common/evaluate_result.h"

namespace SrSecurity {
namespace Operator {
bool DetectSqli::evaluate(Transaction& t, const Common::Variant& operand) const {
  if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
    return false;
  }

  std::string_view data = std::get<std::string_view>(operand);
  char fingerprint[8];
  bool is_sqli = libinjection_sqli(data.data(), data.size(), fingerprint) != 0;
  if (is_sqli) {
    Common::EvaluateResults::Element value;
    value.string_buffer_ = std::string(fingerprint, sizeof(fingerprint));
    value.variant_ = value.string_buffer_;
    t.addCapture(std::move(value));
  }

  return is_sqli;
}
} // namespace Operator
} // namespace SrSecurity