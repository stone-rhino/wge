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

#include <bitset>

namespace Wge {
namespace Transformation {
// Used to store the state of a Ragel stream parser
struct StreamState {
  enum class State {
    // The stream is not valid, it contains invalid data
    INVALID = 1,
    // The stream is complete, no more data to process
    COMPLETE = 2
  };
  std::bitset<8> state_;
  // The current state of the Ragel parser
  int cs_{0};
  // The buffer for neighboring data that is not yet processed
  std::string buffer_;

  StreamState(size_t max_buffer_size) { buffer_.reserve(max_buffer_size); }
};

// The result of stream transformation
enum class StreamResult {
  // The current input was successfully transformed
  SUCCESS = 0,
  // The current input was partially transformed, but more data is needed to complete the
  // transformation
  NEED_MORE_DATA = 1,
  // The current input was not transformed due to invalid input
  INVALID_INPUT = 2
};
} // namespace Transformation
} // namespace Wge