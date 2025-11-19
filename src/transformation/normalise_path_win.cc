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
#include "normalise_path_win.h"

#include <normalize_path_win.h>

namespace Wge {
namespace Transformation {
bool NormalisePathWin::evaluate(std::string_view data, std::string& result) const {
  return normalizePathWin(data, result);
}

std::unique_ptr<StreamState, std::function<void(StreamState*)>>
NormalisePathWin::newStream() const {
  return normalizePathWinNewStream();
}

StreamResult NormalisePathWin::evaluateStream(std::string_view input, std::string& output,
                                              StreamState& state, bool end_stream) const {
  return normalizePathWinStream(input, output, state, end_stream);
}
} // namespace Transformation
} // namespace Wge
