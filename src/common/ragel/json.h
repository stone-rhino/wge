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

#include <forward_list>
#include <list>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "common.h"

#include "../../transformation/stream_util.h"

namespace Wge {
namespace Common {
namespace Ragel {
class Json {
public:
  void init(std::string_view json_str, std::forward_list<std::string>& escape_buffer);

public:
  const std::unordered_multimap<std::string_view, std::string_view>& getKeyValues() const {
    return key_value_map_;
  }
  const std::vector<std::pair<std::string_view, std::string_view>>& getKeyValuesLinked() const {
    return key_value_linked_;
  }

  void clear() {
    key_value_map_.clear();
    key_value_linked_.clear();
  }

public:
  /**
   * Create a new stream state for parsing JSON incrementally.
   * @return A unique pointer to the new stream state.
   */
  static std::unique_ptr<Transformation::StreamState,
                         std::function<void(Transformation::StreamState*)>>
  newStream();

  /**
   * Parse a JSON string incrementally.
   * @param json_str The JSON string to parse.
   * @param key_value_map The map to store key-value pairs. This map is used to store the complete
   * key-value pairs in the JSON string.
   * @param key_value_linked The linked list to store key-value pairs in order. The list contains
   * complete key-value pairs, and the order is the same as in the JSON string. The list may also
   * contain partial key-value pairs that are not yet complete.
   * @param state The current stream state.
   * @param end_stream Indicates if this is the end of the stream.
   * @return The result of the stream parsing.
   * @note The keys and values were escaped using the `jsDecode` function, and the views of
   * keys and values will point to the internal escape buffers. The escape buffers will be freed
   * when call this function again or the stream state is destroyed, so we should carefully use the
   * views.
   */
  static Transformation::StreamResult
  parseStream(std::string_view json_str,
              std::unordered_multimap<std::string_view, std::string_view>& key_value_map,
              std::list<KeyValuePair>& key_value_linked, Transformation::StreamState& state,
              bool end_stream);

private:
  std::unordered_multimap<std::string_view, std::string_view> key_value_map_;
  std::vector<std::pair<std::string_view, std::string_view>> key_value_linked_;
};
} // namespace Ragel
} // namespace Common
} // namespace Wge