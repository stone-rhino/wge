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
  // We don't always use the cache, because the transformation may be too simple, and it's perfmance
  // than search the cache in the hash table.
  // The threshold is 32 bytes, may be it's not a good choice, but it's a good start.
  // TODO(zhouyu 2025-04-11): We can use a better algorithm to choose the threshold.
  constexpr size_t use_cache_limit = 32;
  std::string_view input_data_view = std::get<std::string_view>(input.variant_);
  bool use_cache = input_data_view.size() > use_cache_limit;

  if (use_cache) [[likely]] {
    assert(variable);

    // Check the cache
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
                       std::unordered_map<const char*,
                                          std::optional<Common::EvaluateResults::Element>>{})
              .first;

      iter_data->second.reserve(16);
    }

    // Evaluate the transformation and store the result in the cache
    bool ret = evaluate(input_data_view, output.string_buffer_);
    if (ret) {
      auto iter_transform_result =
          iter_data->second.emplace(name(), Common::EvaluateResults::Element()).first;
      Common::EvaluateResults::Element& result = iter_transform_result->second.value();
      result.string_buffer_ = std::move(output.string_buffer_);
      result.variant_ = result.string_buffer_;
      output.variant_ = result.variant_;
      output.variable_sub_name_ = input.variable_sub_name_;
    } else {
      auto iter_transform_result = iter_data->second.emplace(name(), std::nullopt).first;
    }

    return ret;
  } else {
    bool ret = evaluate(input_data_view, output.string_buffer_);
    if (ret) [[likely]] {
      if (convertToInt()) [[unlikely]] {
        output.variant_ = ::atoi(output.string_buffer_.c_str());
      } else {
        output.variant_ = output.string_buffer_;
      }
      output.variable_sub_name_ = input.variable_sub_name_;
    }

    return ret;
  }
}
} // namespace Transformation
} // namespace Wge
