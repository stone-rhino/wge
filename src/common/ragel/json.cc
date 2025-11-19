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
#include "json.h"

#include <json.h>

namespace Wge {
namespace Common {
namespace Ragel {
void Json::init(std::string_view json_str, std::forward_list<std::string>& escape_buffer) {
  key_value_map_.reserve(32);
  key_value_linked_.reserve(32);
  parseJson(json_str, key_value_map_, key_value_linked_, escape_buffer);
}

std::unique_ptr<Transformation::StreamState, std::function<void(Transformation::StreamState*)>>
Json::newStream() {
  return parseJsonNewStream();
}

Transformation::StreamResult
Json::parseStream(std::string_view json_str,
                  std::unordered_multimap<std::string_view, std::string_view>& key_value_map,
                  std::list<KeyValuePair>& key_value_linked, Transformation::StreamState& state,
                  bool end_stream) {
  return parseJsonStream(json_str, key_value_map, key_value_linked, state, end_stream);
}
} // namespace Ragel
} // namespace Common
} // namespace Wge
