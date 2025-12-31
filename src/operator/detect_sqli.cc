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

namespace Wge {
namespace Operator {
void DetectSqli::evaluate(Transaction& t, const Common::Variant& operand, Results& results) const {
  performComparison<std::string_view, std::string_view>(
      t, operand, "", results,
      [](Transaction& t, std::string_view left_operand, std::string_view right_operand,
         Results& results, void*) {
        char fingerprint[8];
        bool is_sqli =
            libinjection_sqli(left_operand.data(), left_operand.size(), fingerprint) != 0;

        results.emplace_back(is_sqli, t.internString({fingerprint, sizeof(fingerprint)}));
      });
}
} // namespace Operator
} // namespace Wge