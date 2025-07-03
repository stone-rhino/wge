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
#include <functional>
#include <memory>
#include <optional>

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
  std::optional<int> cs_;
  std::optional<int> act_;
  std::optional<size_t> ts_offset_;
  std::optional<size_t> te_offset_;
  // The buffer for neighboring data that is not yet processed
  std::string buffer_;
  // The buffer for extra state. This is used to store additional state information with type
  // erasure. In the specific deleter of smart pointers, we will cast it to the correct type to
  // delete it.
  std::string extra_state_buffer_;

  StreamState(size_t reserve_buffer_size = 0) {
    if (reserve_buffer_size) {
      buffer_.reserve(reserve_buffer_size);
    }
  }
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

inline void recoverStreamState(StreamState& state, std::string_view input, const char*& ps,
                               const char*& pe, const char*& eof, const char*& p, int& cs, int& act,
                               const char*& ts, const char*& te, bool end_stream) {
  // Join the buffer if it is not empty
  if (!state.buffer_.empty()) {
    size_t old_size = state.buffer_.size();
    state.buffer_.append(input.data(), input.size());
    p = state.buffer_.data();
    ps = p;
    pe = p + state.buffer_.size();
    eof = end_stream ? pe : nullptr;

    // We need skip the processed data
    p = ps + old_size;
  }

  // Recover the state
  if (state.cs_.has_value()) {
    cs = state.cs_.value();
  }
  act = state.act_.value_or(0);
  if (state.ts_offset_.has_value()) {
    ts = ps + state.ts_offset_.value();
  }
  if (state.te_offset_.has_value()) {
    te = ps + state.te_offset_.value();
  }
}

inline StreamResult saveStreamState(StreamState& state, int cs, int act, const char* ps,
                                    const char* pe, const char* ts, const char* te,
                                    bool end_stream) {
  if (end_stream) {
    state.buffer_.clear();
    state.state_.set(static_cast<size_t>(Wge::Transformation::StreamState::State::COMPLETE));
    return StreamResult::SUCCESS;
  } else {
    state.cs_ = cs;
    state.act_ = act;
    state.ts_offset_.reset();
    state.te_offset_.reset();

    // If ts is not null, it means ragel in the middle of processing a pattern, we need to
    // save the remaining data in the buffer for the next call.
    if (ts) {
      if (state.buffer_.empty()) {
        state.buffer_.append(ts, pe);
      } else {
        if (ts != state.ts_offset_.value_or(0) + ps) {
          state.buffer_ = state.buffer_.substr(ts - ps);
        }
      }

      // If ts is greater than ps, it means that the last pattern was matched complete,
      // and next pattern is matched from ts now.
      if (ts > ps) {
        ps = ts;
      }

      state.ts_offset_ = ts - ps;
      if (te) {
        state.te_offset_ = te - ps;
      }
    } else {
      // If ts is null, it means that ragel is not in the middle of processing a pattern,
      // we can clear the buffer.
      state.buffer_.clear();
    }
    return StreamResult::NEED_MORE_DATA;
  }
}

template <class T>
inline std::unique_ptr<StreamState, std::function<void(StreamState*)>> newStreamWithExtraState() {
  auto state = std::unique_ptr<Wge::Transformation::StreamState,
                               std::function<void(Wge::Transformation::StreamState*)>>(
      new Wge::Transformation::StreamState(), [](Wge::Transformation::StreamState* state) {
        T* extra_state = reinterpret_cast<T*>(state->extra_state_buffer_.data());
        extra_state->~T();
        delete state;
      });

  state->extra_state_buffer_.resize(sizeof(T));
  T* extra_state = reinterpret_cast<T*>(state->extra_state_buffer_.data());
  new (extra_state) T();

  return state;
}
} // namespace Transformation
} // namespace Wge