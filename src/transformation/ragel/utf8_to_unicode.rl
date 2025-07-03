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

// clang-format off
%%{
  machine utf8_to_unicode;
  
  action skip {}

  action exec_transformation { 
    result.resize(input.size() * 2);
    r = result.data();
    if(ts > input.data()){
      memcpy(r, input.data(), ts - input.data());
      r += ts - input.data();
    }
    result.resize(r - result.data());
    p = ts;
    fhold;
    fgoto transformation;
  }

  action append_ascii {
    result += fc;
  }

  action append_two_byte {
    uint32_t unicode = ((ts[0] & 0x1F) << 6) | (ts[1] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  action append_three_byte {
    uint32_t unicode = ((ts[0] & 0x0F) << 12) | ((ts[1] & 0x3F) << 6) | (ts[2] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  action append_four_byte {
    uint32_t unicode = ((ts[0] & 0x07) << 18) | ((ts[1] & 0x3F) << 12) | ((ts[2] & 0x3F) << 6) | (ts[3] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  # prescan
  main := |*
    0x00..0x7F => skip;
    0xC2..0xDF 0x80..0xBF => exec_transformation;
    0xE0..0xEF 0x80..0xBF 0x80..0xBF => exec_transformation;
    0xF0..0xF4 0x80..0xBF 0x80..0xBF 0x80..0xBF => exec_transformation;
    any => { fbreak; };
  *|;
  
  transformation := |*
    0x00..0x7F => append_ascii;
    0xC2..0xDF 0x80..0xBF => append_two_byte;
    0xE0..0xEF 0x80..0xBF 0x80..0xBF => append_three_byte;
    0xF0..0xF4 0x80..0xBF 0x80..0xBF 0x80..0xBF => append_four_byte;
    any => skip;
  *|;
}%%

%% write data;
// clang-format on

static constexpr std::string_view table{"0123456789abcdef"};

static bool utf8ToUnicode(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  if (r) {
    return true;
  }

  return false;
}

// clang-format off
%%{
  machine utf8_to_unicode_stream;
  
  action skip {}

  action append_ascii {
    result += fc;
  }

  action append_two_byte {
    uint32_t unicode = ((ts[0] & 0x1F) << 6) | (ts[1] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  action append_three_byte {
    uint32_t unicode = ((ts[0] & 0x0F) << 12) | ((ts[1] & 0x3F) << 6) | (ts[2] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  action append_four_byte {
    uint32_t unicode = ((ts[0] & 0x07) << 18) | ((ts[1] & 0x3F) << 12) | ((ts[2] & 0x3F) << 6) | (ts[3] & 0x3F);
    result += "%u";
    result += table[(unicode >> 12) & 0x0F];
    result += table[(unicode >> 8) & 0x0F];
    result += table[(unicode >> 4) & 0x0F];
    result += table[unicode & 0x0F];
  }

  main := |*
    0x00..0x7F => append_ascii;
    0xC2..0xDF 0x80..0xBF => append_two_byte;
    0xE0..0xEF 0x80..0xBF 0x80..0xBF => append_three_byte;
    0xF0..0xF4 0x80..0xBF 0x80..0xBF 0x80..0xBF => append_four_byte;
    any => skip;
  *|;
}%%

%% write data;
// clang-format on

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
utf8ToUnicodeNewStream() {
  return std::make_unique<Wge::Transformation::StreamState>();
}

static Wge::Transformation::StreamResult
utf8ToUnicodeStream(std::string_view input, std::string& result,
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

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}