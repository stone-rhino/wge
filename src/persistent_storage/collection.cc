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
#include "collection.h"

#include <time.h>

namespace Wge {
namespace PersistentStorage {
Collection::Collection() { create_time_ = ::time(nullptr); }

void Collection::set(const std::string& key, const Common::Variant& value) {
  std::lock_guard<std::mutex> lock(kv_mutex_);
  auto iter = kv_.find(key);
  if (iter == kv_.end()) {
    auto result = kv_.try_emplace(key);
    iter = result.first;
  }

  iter->second.variant_ = value;
  if (IS_STRING_VIEW_VARIANT(value)) {
    iter->second.string_buffer_ = std::get<std::string_view>(value);
    iter->second.variant_ = iter->second.string_buffer_;
  }

  last_update_time_ = ::time(nullptr);
  ++update_counter_;
}

const Common::Variant& Collection::get(const std::string& key) const {
  std::lock_guard<std::mutex> lock(kv_mutex_);
  auto iter = kv_.find(key);
  if (iter != kv_.end()) {
    return iter->second.variant_;
  }

  return EMPTY_VARIANT;
}

void Collection::travel(
    std::function<bool(const std::string&, const Common::Variant&)> func) const {
  std::lock_guard<std::mutex> lock(kv_mutex_);
  for (const auto& [key, value] : kv_) {
    if (!func(key, value.variant_)) {
      break;
    }
  }
}
} // namespace PersistentStorage
} // namespace Wge