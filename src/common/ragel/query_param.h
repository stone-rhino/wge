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

#include <string_view>
#include <unordered_map>
#include <vector>

namespace SrSecurity {
namespace Common {
namespace Ragel {
class QueryParam {
public:
  void init(std::string_view query_param_str);

public:
  const std::unordered_multimap<std::string_view, std::string_view>& get() const {
    return query_param_map_;
  }

  const std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>&
  getLinked() const {
    return query_param_linked_;
  }

private:
  std::unordered_multimap<std::string_view, std::string_view> query_param_map_;
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator> query_param_linked_;
};
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity