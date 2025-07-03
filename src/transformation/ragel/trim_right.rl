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

#include <string>
#include <string_view>

#include "src/transformation/stream_util.h"

// The Ragel does not support reverse traversal, so we implement the trim right function manually.
static bool trimRight(std::string_view input, std::string& result) {
  result.clear();

  if (input.empty()) {
    return false;
  }

  constexpr auto white_space = " \t\n\r\f\v";

  size_t pos = input.find_last_not_of(white_space);

  // The characters in the string are all spaces
  if (pos == std::string_view::npos) {
    return true;
  }

  if (pos == input.size() - 1) {
    return false;
  }

  result = input.substr(0, pos + 1);

  return true;
}

// clang-format off
%%{
  machine trim_right_stream;

  action skip {}

  WS = [ \t\n\r\f\v];

  main := |*
    WS => { 
      whitespace_count++; 
      fgoto whitespace; 
    };
    any => {
      result += fc;
    };
  *|;
  
  # In the whitespace state, we count the number of consecutive spaces.
  # This is support infinite whitespace at the end of the stream. But the 
  # all withspace characters will transformed to ' ' character.
  whitespace := |*
    WS => { whitespace_count++; };
    any => {
      result.append(whitespace_count, ' ');
      result += fc;
      whitespace_count = 0;
      fgoto main;  
    };
  *|;
}%%
%% write data;
// clang-format on

struct TrimRightStreamExtraState {
  // The size of the processed data
  size_t processed_size_{0};

  // The count of consecutive spaces at the end of the stream
  size_t consecutive_space_count_{0};
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
trimRightNewStream() {
  return Wge::Transformation::newStreamWithExtraState<TrimRightStreamExtraState>();
}

static Wge::Transformation::StreamResult trimRightStream(std::string_view input,
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

  TrimRightStreamExtraState* extra_state =
      reinterpret_cast<TrimRightStreamExtraState*>(state.extra_state_buffer_.data());

  size_t& whitespace_count = extra_state->consecutive_space_count_;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}