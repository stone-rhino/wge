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
#include "set_env.h"

#include <stdlib.h>

#include "../common/assert.h"

namespace Wge {
namespace Action {
SetEnv::SetEnv(std::string&& key, std::string&& value)
    : key_(std::move(key)), value_(std::move(value)) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

SetEnv::SetEnv(std::string&& key, std::shared_ptr<Macro::MacroBase> macro)
    : key_(std::move(key)), macro_(macro) {
  // The variable name is case insensitive
  std::transform(key_.begin(), key_.end(), key_.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void SetEnv::evaluate(Transaction& t) const {
  if (macro_) {
    Common::EvaluateResults result;
    macro_->evaluate(t, result);
    if (IS_INT_VARIANT(result.front().variant_)) {
      ::setenv(key_.c_str(), std::to_string(std::get<int>(result.front().variant_)).c_str(), 1);
    } else if (IS_STRING_VIEW_VARIANT(result.front().variant_)) {
      std::string value_str(std::get<std::string_view>(result.front().variant_));
      ::setenv(key_.c_str(), value_str.c_str(), 1);
    } else {
      UNREACHABLE();
    }
  } else {
    ::setenv(key_.c_str(), value_.c_str(), 1);
  }
}
} // namespace Action
} // namespace Wge