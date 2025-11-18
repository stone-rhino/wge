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
#include "transform_base.h"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace Wge {
namespace Transformation {
bool TransformBase::evaluate(Transaction& t, const Variable::VariableBase* variable,
                             const Common::EvaluateElement& input,
                             Common::EvaluateElement& output) const {
  assert(variable);

  // Check the cache
  std::string_view input_data_view = std::get<std::string_view>(input.variant_);
  auto& transform_cache = t.getTransformCache();
  Transaction::TransformCacheKey cache_key(input_data_view, name());
  auto iter = transform_cache.find(cache_key);
  if (iter != transform_cache.end())
    [[likely]] {
      WGE_LOG_TRACE(
          "transform cache hit: {} {}",
          [&]() {
            if (input.variable_sub_name_.empty()) {
              return std::string(variable->fullName().main_name_);
            } else {
              return std::format("{}:{}", variable->fullName().main_name_,
                                 input.variable_sub_name_);
            }
          }(),
          name());

      // The transformation has been evaluated before.
      if (iter->second)
        [[likely]] {
          output.variant_ = iter->second->variant_;
          output.variable_sub_name_ = input.variable_sub_name_;
          return true;
        }
      else {
        return false;
      }
    }

  // Evaluate the transformation and store the result in the cache
  std::string output_buffer;
  bool ret = evaluate(input_data_view, output_buffer);
  if (ret) {
    auto iter_transform_result =
        transform_cache.emplace(cache_key, t.internString(std::move(output_buffer))).first;
    output.variant_ = iter_transform_result->second->variant_;
    output.variable_sub_name_ = input.variable_sub_name_;
  } else {
    // Store nullopt to indicate failure
    transform_cache.emplace(cache_key, std::nullopt);
  }

  return ret;
}
} // namespace Transformation
} // namespace Wge
