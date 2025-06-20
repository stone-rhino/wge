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

#include <memory>
#include <string>
#include <string_view>

#include <assert.h>

#include "src/transformation/stream_state.h"

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
    '=' => { 
      if(has_equal_sign) {
        *has_equal_sign = true;
      } 
      fbreak;
    };
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

static bool base64Decode(std::string_view input, std::string& result,
                         std::string* remaining = nullptr, bool* has_equal_sign = nullptr) {
  result.clear();
  if (remaining) {
    remaining->clear();
  }

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

  if (!result.empty()) {
    // Process remaining bytes
    if (!remaining) {
      if (count == 2) {
        *r++ = (buffer >> 4) & 0xFF;
      } else if (count == 3) {
        *r++ = (buffer >> 10) & 0xFF;
        *r++ = (buffer >> 2) & 0xFF;
      }
    } else {
      // Encode remaining bytes to base64 format
      static constexpr std::string_view base64encode_table =
          "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
      if (count == 2) {
        remaining->push_back(base64encode_table[(buffer >> 6) & 0x3F]);
        remaining->push_back(base64encode_table[buffer & 0x3F]);
      } else if (count == 3) {
        remaining->push_back(base64encode_table[(buffer >> 12) & 0x3F]);
        remaining->push_back(base64encode_table[(buffer >> 6) & 0x3F]);
        remaining->push_back(base64encode_table[buffer & 0x3F]);
      }
    }

    result.resize(r - result.data());
    return true;
  }

  return false;
}

static std::unique_ptr<Wge::Transformation::StreamState> base64DecodeStreamStart() {
  return std::make_unique<Wge::Transformation::StreamState>(4);
}

static Wge::Transformation::StreamResult
base64DecodeStream(std::string_view input, std::string& result,
                   Wge::Transformation::StreamState& state) {
  using namespace Wge::Transformation;

  // A valid base64 data must be multiple of 4 bytes, so we need at least 4 bytes to process a data
  // slice
  static constexpr size_t MIN_DATA_SIZE = 4;

  // The stream is not valid
  if (state.state_.test(static_cast<size_t>(StreamState::State::INVALID)))
    [[unlikely]] { return StreamResult::INVALID_INPUT; }

  // The stream is complete, no more data to process
  if (state.state_.test(static_cast<size_t>(StreamState::State::COMPLETE)))
    [[unlikely]] { return StreamResult::SUCCESS; }

  // Process the buffered data first
  if (!state.buffer_.empty()) {
    std::string remaining;
    do {
      // Fill the buffer to full 4-byte chunk
      size_t needed = MIN_DATA_SIZE - state.buffer_.size();
      if (input.size() < needed) {
        // Not enough data to fill the buffer, append what we have and return
        state.buffer_.append(input);
        return StreamResult::NEED_MORE_DATA;
      }
      state.buffer_.append(input.substr(0, needed));
      input.remove_prefix(needed);

      // Decode the buffered data
      std::string temp_result;
      bool has_equal_sign = false;
      if (base64Decode(state.buffer_, temp_result, &remaining, &has_equal_sign)) {
        if (has_equal_sign) {
          state.state_.set(static_cast<size_t>(StreamState::State::COMPLETE));
        }

        result.append(temp_result);

        if (!remaining.empty()) {
          state.buffer_ = remaining;
        } else {
          state.buffer_.clear();
        }

      } else {
        state.buffer_.clear();
        state.state_.set(static_cast<size_t>(StreamState::State::INVALID));
        return StreamResult::INVALID_INPUT;
      }
    } while (!remaining.empty());
  }

  // Not enough data to process
  if (input.size() < MIN_DATA_SIZE) {
    state.buffer_.append(input);
    return StreamResult::NEED_MORE_DATA;
  }

  // Now process the input data
  std::string temp_result;
  std::string remaining;
  bool has_equal_sign = false;
  if (base64Decode(input, temp_result, &remaining, &has_equal_sign)) {
    if (has_equal_sign) {
      state.state_.set(static_cast<size_t>(StreamState::State::COMPLETE));
    }

    result.append(temp_result);
    state.buffer_.clear();

    if (!remaining.empty()) {
      state.buffer_.append(remaining);
      return StreamResult::NEED_MORE_DATA;
    }
  } else {
    state.state_.set(static_cast<size_t>(StreamState::State::INVALID));
    return StreamResult::INVALID_INPUT;
  }

  return StreamResult::SUCCESS;
}

static void base64DecodeStreamStop(std::string& result, Wge::Transformation::StreamState& state) {
  // We process the remaining buffered data possibly left in the state
  if (!state.buffer_.empty()) {
    std::string temp_result;
    if (base64Decode(state.buffer_, temp_result)) {
      result.append(temp_result);
    }
  }

  state.state_.set(static_cast<size_t>(Wge::Transformation::StreamState::State::COMPLETE));
}