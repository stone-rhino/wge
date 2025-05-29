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
                             const Common::EvaluateResults::Element& input,
                             Common::EvaluateResults::Element& output) const {
  assert(variable);

  // Check the cache
  std::string_view input_data_view = std::get<std::string_view>(input.variant_);
  auto& transform_cache = t.getTransformCache();
  auto iter_data = transform_cache.find(input_data_view);
  if (iter_data != transform_cache.end()) {
    auto iter_transform_result = iter_data->second.find(name());
    if (iter_transform_result != iter_data->second.end()) {
      if (!input.variable_sub_name_.empty()) [[unlikely]] {
        WGE_LOG_TRACE("transform cache hit: {}:{} {}", variable->fullName().main_name_,
                      input.variable_sub_name_, name());
      } else {
        WGE_LOG_TRACE("transform cache hit: {} {}", variable->fullName().main_name_, name());
      }

      // The transformation has been evaluated before.
      if (iter_transform_result->second.has_value()) [[likely]] {
        output.variant_ = iter_transform_result->second.value().variant_;
        output.variable_sub_name_ = input.variable_sub_name_;
        return true;
      } else {
        return false;
      }
    }
  } else {
    iter_data =
        transform_cache
            .emplace(input_data_view,
                     boost::unordered_flat_map<const char*,
                                               std::optional<Common::EvaluateResults::Element>>{})
            .first;

    iter_data->second.reserve(16);
  }

  // Evaluate the transformation and store the result in the cache
  std::string output_buffer;
  bool ret = evaluate(input_data_view, output_buffer);
  if (ret) {
    auto iter_transform_result =
        iter_data->second.emplace(name(), Common::EvaluateResults::Element()).first;
    Common::EvaluateResults::Element& result = iter_transform_result->second.value();
    result.string_buffer_ = std::move(output_buffer);
    result.variant_ = result.string_buffer_;
    output.variant_ = result.variant_;
    output.variable_sub_name_ = input.variable_sub_name_;
  } else {
    auto iter_transform_result = iter_data->second.emplace(name(), std::nullopt).first;
  }

  return ret;
}
} // namespace Transformation
} // namespace Wge
