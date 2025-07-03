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

#include <string.h>

#include "src/transformation/stream_util.h"

// clang-format off
%%{
  machine hex_decode;
  
  action exec_transformation { 
    result.resize(input.size() / 2 + 1);
    r = result.data();
    if(ts > input.data()){
      memcpy(r, input.data(), ts - input.data());
      r += ts - input.data();
    }
    p = ts;
    fhold;
    fgoto transformation;
  }

  # prescan
  main := |*
    [0-9a-fA-F] => exec_transformation;
    any => { fbreak; };
  *|;

  transformation := |*
    '0' => { if((p - ps) % 2 == 0){*r++ = 0 << 4;} else { *(r-1) |= 0; } };
    '1' => { if((p - ps) % 2 == 0){*r++ = 1 << 4;} else { *(r-1) |= 1; } };
    '2' => { if((p - ps) % 2 == 0){*r++ = 2 << 4;} else { *(r-1) |= 2; } };
    '3' => { if((p - ps) % 2 == 0){*r++ = 3 << 4;} else { *(r-1) |= 3; } };
    '4' => { if((p - ps) % 2 == 0){*r++ = 4 << 4;} else { *(r-1) |= 4; } };
    '5' => { if((p - ps) % 2 == 0){*r++ = 5 << 4;} else { *(r-1) |= 5; } };
    '6' => { if((p - ps) % 2 == 0){*r++ = 6 << 4;} else { *(r-1) |= 6; } };
    '7' => { if((p - ps) % 2 == 0){*r++ = 7 << 4;} else { *(r-1) |= 7; } };
    '8' => { if((p - ps) % 2 == 0){*r++ = 8 << 4;} else { *(r-1) |= 8; } };
    '9' => { if((p - ps) % 2 == 0){*r++ = 9 << 4;} else { *(r-1) |= 9; } };
    [aA] => { if((p - ps) % 2 == 0){*r++ = 10 << 4;} else { *(r-1) |= 10; } };
    [bB] => { if((p - ps) % 2 == 0){*r++ = 11 << 4;} else { *(r-1) |= 11; } };
    [cC] => { if((p - ps) % 2 == 0){*r++ = 12 << 4;} else { *(r-1) |= 12; } };
    [dD] => { if((p - ps) % 2 == 0){*r++ = 13 << 4;} else { *(r-1) |= 13; } };
    [eE] => { if((p - ps) % 2 == 0){*r++ = 14 << 4;} else { *(r-1) |= 14; } };
    [fF] => { if((p - ps) % 2 == 0){*r++ = 15 << 4;} else { *(r-1) |= 15; } };
    any => { fhold; fbreak; };
  *|;

}%%

%% write data;
// clang-format on

static bool hexDecode(std::string_view input, std::string& result) {
  result.clear();
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

    // If the input length is odd, we assume the last character is a low 4 bits of a byte
    if ((p - ps) % 2 != 0) {
      result.back() = result.back() >> 4 & 0x0F;
    }
    return true;
  }

  return false;
}

// clang-format off
%%{
  machine hex_decode_stream;

  main := |*
    '0' => { if(count % 2 == 0){result += 0 << 4;} else { result.back() |= 0; } ++count; };
    '1' => { if(count % 2 == 0){result += 1 << 4;} else { result.back() |= 1; } ++count; };
    '2' => { if(count % 2 == 0){result += 2 << 4;} else { result.back() |= 2; } ++count; };
    '3' => { if(count % 2 == 0){result += 3 << 4;} else { result.back() |= 3; } ++count; };
    '4' => { if(count % 2 == 0){result += 4 << 4;} else { result.back() |= 4; } ++count; };
    '5' => { if(count % 2 == 0){result += 5 << 4;} else { result.back() |= 5; } ++count; };
    '6' => { if(count % 2 == 0){result += 6 << 4;} else { result.back() |= 6; } ++count; };
    '7' => { if(count % 2 == 0){result += 7 << 4;} else { result.back() |= 7; } ++count; };
    '8' => { if(count % 2 == 0){result += 8 << 4;} else { result.back() |= 8; } ++count; };
    '9' => { if(count % 2 == 0){result += 9 << 4;} else { result.back() |= 9; } ++count; };
    [aA] => { if(count % 2 == 0){result += 10 << 4;} else { result.back() |= 10; } ++count; };
    [bB] => { if(count % 2 == 0){result += 11 << 4;} else { result.back() |= 11; } ++count; };
    [cC] => { if(count % 2 == 0){result += 12 << 4;} else { result.back() |= 12; } ++count; };
    [dD] => { if(count % 2 == 0){result += 13 << 4;} else { result.back() |= 13; } ++count; };
    [eE] => { if(count % 2 == 0){result += 14 << 4;} else { result.back() |= 14; } ++count; };
    [fF] => { if(count % 2 == 0){result += 15 << 4;} else { result.back() |= 15; } ++count; };
    any => {
      // If the input length is odd, we assume the last character is a low 4 bits of a byte
      if (count % 2 != 0) {
        result.back() = result.back() >> 4 & 0x0F;
      }
      state.state_.set(static_cast<size_t>(Wge::Transformation::StreamState::State::COMPLETE));
      fbreak;
    };
  *|;

}%%

%% write data;
//clang-format on

struct HexDecodeExtraState {
  // Count of processed hex characters
  int count_{0};
};

static std::unique_ptr<Wge::Transformation::StreamState, std::function<void(Wge::Transformation::StreamState*)>> hexDecodeNewStream() {
  return Wge::Transformation::newStreamWithExtraState<HexDecodeExtraState>();
}

static Wge::Transformation::StreamResult
hexDecodeStream(std::string_view input, std::string& result,
                      Wge::Transformation::StreamState& state, bool end_stream) {
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

  auto* extra_state = reinterpret_cast<HexDecodeExtraState*>(state.extra_state_buffer_.data());
  int& count = extra_state->count_;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  if (end_stream) {
    // If the input length is odd, we assume the last character is a low 4 bits of a byte
    if (count % 2 != 0) {
      result.back() = result.back() >> 4 & 0x0F;
    }
  }

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}