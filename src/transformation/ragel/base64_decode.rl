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

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>

#include <assert.h>

#include "src/transformation/stream_util.h"

// clang-format off
%%{
  machine base64_decode;
  
  action skip {}

  action decode_char {
    buffer = (buffer << 6) | base64_table[fc];
    count++;
    if (count == 4) {
      *r++ = (buffer >> 16) & 0xFF;
      *r++ = (buffer >> 8) & 0xFF;
      *r++ = buffer & 0xFF;
      buffer = 0;
      count = 0;
    }
  }

  main := |*
    [+/0-9A-Za-z] => decode_char;
    '=' => { fbreak; };
    any => skip;
  *|;
}%%

%% write data;
// clang-format on

static const char base64_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32-47
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48-63
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, // 64-79
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80-95
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96-111
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112-127
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // 240-255
};

static bool base64Decode(std::string_view input, std::string& result) {
  result.clear();

  const char* p = input.data();
  const char* pe = p + input.size();

  result.resize(input.size() / 4 * 3 + 3);
  char* r = result.data();

  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

  unsigned int buffer = 0;
  int count = 0;

  // clang-format off
  %% write init;
  %% write exec;
  // clang-format on

  // Process remaining bytes
  if (count == 2) {
    *r++ = (buffer >> 4) & 0xFF;
  } else if (count == 3) {
    *r++ = (buffer >> 10) & 0xFF;
    *r++ = (buffer >> 2) & 0xFF;
  }

  result.resize(r - result.data());
  return true;
}

// clang-format off
%%{
  machine base64_decode_stream;
  
  action skip {}

  action decode_char {
    extra_state->buffer_ = (extra_state->buffer_ << 6) | base64_table[fc];
    extra_state->count_++;
    if (extra_state->count_ == 4) {
      result.reserve(result.size() + 3);
      result += (extra_state->buffer_ >> 16) & 0xFF;
      result += (extra_state->buffer_ >> 8) & 0xFF;
      result += extra_state->buffer_ & 0xFF;
      extra_state->buffer_ = 0;
      extra_state->count_ = 0;
    }
  }

  action stream_complete {
    // Process remaining bytes
    if (extra_state->count_ == 2) {
      result += (extra_state->buffer_ >> 4) & 0xFF;
    } else if (extra_state->count_ == 3) {
      result += (extra_state->buffer_ >> 10) & 0xFF;
      result += (extra_state->buffer_ >> 2) & 0xFF;
    }
    extra_state->buffer_ = 0;
    extra_state->count_ = 0;
    state.state_.set(static_cast<size_t>(Wge::Transformation::StreamState::State::COMPLETE));
    fbreak;
  }

  main := |*
    [+/0-9A-Za-z] => decode_char;
    '=' => stream_complete;
    any => skip;
  *|;
}%%

%% write data;
// clang-format on

struct Base64DecodeExtraState {
  unsigned int buffer_{0};
  int count_{0};
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
base64DecodeNewStream() {
  return Wge::Transformation::newStreamWithExtraState<Base64DecodeExtraState>();
}

static Wge::Transformation::StreamResult base64DecodeStream(std::string_view input,
                                                            std::string& result,
                                                            Wge::Transformation::StreamState& state,
                                                            bool end_stream) {
  using namespace Wge::Transformation;

  // The stream is not valid
  if (state.state_.test(static_cast<size_t>(StreamState::State::INVALID)))
    [[unlikely]] { return StreamResult::INVALID_INPUT; }

  // The stream is complete, no more data to process
  if (state.state_.test(static_cast<size_t>(StreamState::State::COMPLETE)))
    [[unlikely]] { return StreamResult::SUCCESS; }

  // In the stream mode, we can't operate the raw pointer of the result directly simular to the
  // block mode since we can't guarantee reserve enough space in the result string. Instead, we
  // will use the string's append method to add the transformed data. Although this is less
  // efficient than using a raw pointer, it is necessary to ensure the safety of the stream
  // processing.
  result.reserve(result.size() + input.size());

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = end_stream ? pe : nullptr;
  const char *ts, *te;
  int cs, act;

  auto* extra_state = reinterpret_cast<Base64DecodeExtraState*>(state.extra_state_buffer_.data());

  // clang-format off
  %% write init;

  // Recover the state
  if(state.cs_.has_value()) {
    cs = state.cs_.value();
  }
  act = state.act_.value_or(0);
  if(state.ts_offset_.has_value()) {
    ts = ps + state.ts_offset_.value();
  }
  if(state.te_offset_.has_value()) {
    te = ps + state.te_offset_.value();
  }
  
  %% write exec;
  // clang-format on

  if (end_stream) {
    if (extra_state->count_ == 2) {
      result += (extra_state->buffer_ >> 4) & 0xFF;
    } else if (extra_state->count_ == 3) {
      result += (extra_state->buffer_ >> 10) & 0xFF;
      result += (extra_state->buffer_ >> 2) & 0xFF;
    }
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
    }
    return StreamResult::NEED_MORE_DATA;
  }
}
