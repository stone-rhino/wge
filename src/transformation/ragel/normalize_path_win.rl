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

#include <normalize_path.h>

#include "src/transformation/stream_util.h"

// Same as normalizePath, but first converts backslash characters to forward slashes.
// clang-format off
%%{
  machine normalize_path_win;

  action skip {}

  action exec_transformation { 
    result.resize(input.size());
    r = result.data();
    if(ts > input.data()){
      memcpy(r, input.data(), ts - input.data());
      r += ts - input.data();
    }
    p = ts;
    fhold;
    fgoto transformation;
  }

  SLASH = '\\';

  # prescan
  main := |*
    SLASH => exec_transformation;
    any => skip;
  *|;

  transformation := |*
    SLASH => { *r++ = '/'; };
    any => { *r++ = fc; };
  *|;
}%%

%% write data;
// clang-format on

static bool normalizePathWin(std::string_view input, std::string& result2) {

  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  if (r) {
    result.resize(r - result.data());
    bool ret = ::normalizePath(result, result2);
    if (!ret) {
      result2 = std::move(result);
    }

    return true;
  } else {
    return normalizePath(input, result2);
  }
}

// clang-format off
%%{
  machine normalize_path_win_stream;

  SLASH = '\\';

  main := |*
    SLASH => { chunk_result += '/'; };
    any => { chunk_result += fc; };
  *|;
}%%

%% write data;
// clang-format on

struct NormalizePathWinExtraState {
  std::unique_ptr<Wge::Transformation::StreamState,
                  std::function<void(Wge::Transformation::StreamState*)>>
      normalize_path_state_;
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
normalizePathWinNewStream() {
  auto state = Wge::Transformation::newStreamWithExtraState<NormalizePathWinExtraState>();
  NormalizePathWinExtraState* extra_state =
      reinterpret_cast<NormalizePathWinExtraState*>(state->extra_state_buffer_.data());
  extra_state->normalize_path_state_ = normalizePathNewStream();

  return state;
}

static Wge::Transformation::StreamResult
normalizePathWinStream(std::string_view input, std::string& result,
                       Wge::Transformation::StreamState& state, bool end_stream) {
  using namespace Wge::Transformation;

  // The stream is not valid
  if (state.state_.test(static_cast<size_t>(StreamState::State::INVALID)))
    [[unlikely]] { return StreamResult::INVALID_INPUT; }

  // The stream is complete, no more data to process
  if (state.state_.test(static_cast<size_t>(StreamState::State::COMPLETE)))
    [[unlikely]] { return StreamResult::SUCCESS; }

  std::string chunk_result;
  chunk_result.reserve(input.size());

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = end_stream ? pe : nullptr;
  const char *ts, *te;
  int cs, act;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);

  std::unique_ptr<Wge::Transformation::StreamState,
                  std::function<void(Wge::Transformation::StreamState*)>>* second_stream_state =
      reinterpret_cast<std::unique_ptr<Wge::Transformation::StreamState,
                                       std::function<void(Wge::Transformation::StreamState*)>>*>(
          state.extra_state_buffer_.data());
  return normalizePathStream(chunk_result, result, *(second_stream_state->get()), end_stream);
}