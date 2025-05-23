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

#include <functional>
#include <string_view>

namespace Wge {

/**
 * Header find function.
 * @param lower_case_key the header key with lower case.
 * @return the header value. if the header does not exist, return empty string_view.
 */
using HeaderFind = std::function<std::string_view(const std::string& lower_case_key)>;

/**
 * Header traversal callback.
 * @param key the header key.
 * @param lower_case_key the header key with lower case.
 * @return true if continue traversal, false if stop traversal.
 */
using HeaderTraversalCallback =
    std::function<bool(std::string_view lower_case_key, std::string_view value)>;

/**
 * Header traversal function.
 * @param callback the header traversal callback.
 */
using HeaderTraversal = std::function<void(HeaderTraversalCallback call)>;

/**
 * Http message info extractor
 */
struct HttpExtractor {
  HeaderFind request_header_find_;
  HeaderTraversal request_header_traversal_;
  HeaderFind response_header_find_;
  HeaderTraversal response_header_traversal_;
  size_t request_header_count_;
  size_t response_header_count_;
};
} // namespace Wge