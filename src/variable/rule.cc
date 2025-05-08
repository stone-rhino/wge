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
#include "rule.h"

#include "../rule.h"

namespace Wge {
namespace Variable {
void Rule::initEvaluateFunc() {
  static const std::unordered_map<std::string,
                                  std::function<void(Transaction&, Common::EvaluateResults&, bool)>>
      evaluate_func_map = {
          {"id",
           [](Transaction& t, Common::EvaluateResults& result, bool is_count) {
             if (is_count) {
               result.append(t.currentEvaluateRule()->id() == 0 ? 0 : 1);
               return;
             }

             result.append(static_cast<int>(t.currentEvaluateRule()->id()), "id");
           }},
          {"phase",
           [](Transaction& t, Common::EvaluateResults& result, bool is_count) {
             if (is_count) {
               result.append(t.currentEvaluateRule()->phase() == -1 ? 0 : 1);
               return;
             }

             result.append(t.currentEvaluateRule()->phase(), "phase");
           }},
          {"operator_value", [](Transaction& t, Common::EvaluateResults& result, bool is_count) {
             if (is_count) {
               if (t.currentEvaluateRule()->getOperator()->literalValue().empty() &&
                   t.currentEvaluateRule()->getOperator()->macro() == nullptr) {
                 result.append(0, "operator_value");
               } else {
                 result.append(1, "operator_value");
               }

               return;
             }

             if (!t.currentEvaluateRule()->getOperator()->literalValue().empty()) {
               result.append(t.currentEvaluateRule()->getOperator()->literalValue(),
                             "operator_value");
             }
           }}};
  std::string sub_name_ignore_case;
  sub_name_ignore_case.reserve(sub_name_.size());
  std::transform(sub_name_.begin(), sub_name_.end(), std::back_inserter(sub_name_ignore_case),
                 ::tolower);
  auto iter = evaluate_func_map.find(sub_name_ignore_case);
  if (iter != evaluate_func_map.end()) {
    evaluate_func_ = iter->second;
  }
}
} // namespace Variable
} // namespace Wge