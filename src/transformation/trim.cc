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
#include "trim.h"

#include <trim_left.h>
#include <trim_right.h>

namespace Wge {
namespace Transformation {
bool Trim::evaluate(std::string_view data, std::string& result) const {
  std::string left_result;
  auto ret1 = trimLeft(data, left_result);

  std::string_view right_input = ret1 ? left_result : data;
  auto ret2 = trimRight(right_input, result);

  return ret1 || ret2;
}

std::unique_ptr<StreamState, std::function<void(StreamState*)>> Trim::newStream() const {
  auto state = std::unique_ptr<StreamState, std::function<void(StreamState*)>>(
      new StreamState(), [](StreamState* state) {
        TrimStreamExtraState* extra_state =
            reinterpret_cast<TrimStreamExtraState*>(state->extra_state_buffer_.data());
        extra_state->left_state_->~StreamState();
        extra_state->right_state_->~StreamState();
        delete state;
      });

  state->extra_state_buffer_.resize(sizeof(TrimStreamExtraState));
  TrimStreamExtraState* extra_state =
      reinterpret_cast<TrimStreamExtraState*>(state->extra_state_buffer_.data());
  extra_state->left_state_ = trimLeftNewStream();
  extra_state->right_state_ = trimRightNewStream();

  return state;
}

StreamResult Trim::evaluateStream(std::string_view input, std::string& output, StreamState& state,
                                  bool end_stream) const {
  TrimStreamExtraState* extra_state =
      reinterpret_cast<TrimStreamExtraState*>(state.extra_state_buffer_.data());
  std::string left_result;
  auto result = trimLeftStream(input, left_result, *(extra_state->left_state_.get()), end_stream);

  result = trimRightStream(left_result, output, *(extra_state->right_state_.get()), end_stream);

  return result;
}
} // namespace Transformation
} // namespace Wge
