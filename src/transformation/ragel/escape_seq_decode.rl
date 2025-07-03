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

#include "hex_decode.h"
#include "src/transformation/stream_util.h"

// Decodes ANSI C escape sequences: \a, \b, \f, \n, \r, \t, \v, \\, \?, \', \", \xHH (hexadecimal),
// \0OOO (octal). Invalid encodings are left in the output.
// clang-format off
%%{
  machine escape_seq_decode;
  
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

  action decode_hex {
    std::string decode;
    if(hexDecode({ts + 2, 2},decode) && !decode.empty()){
      *r++ = decode.front();
    }
  }

  action decode_octal {
    unsigned char value = 0;
    const char* p_octal = ts + 1;

    // The first octal
    value = (*p_octal++ - '0');

    // The second octal
    if (p_octal < te) {
      value = (value << 3) | (*p_octal++ - '0'); 
    }

    // The third octal
    if (p_octal < te) {
      value = (value << 3) | (*p_octal++ - '0'); 
    }

    *r++ = value;
  }

  # prescan
  main := |*
    '\\' => exec_transformation;
    any => {};
  *|;
  
  hex = [0-9a-fA-F];
  octal = [0-7];
  transformation := |*
    '\\x' hex hex => decode_hex;
    '\\' octal octal? octal? => decode_octal;
    '\\a'  => { *r++ = '\a'; };
    '\\b' => { *r++ = '\b'; };
    '\\f' => { *r++ = '\f'; };
    '\\n' => { *r++ = '\n'; };
    '\\r' => { *r++ = '\r'; };
    '\\t' => { *r++ = '\t'; };
    '\\v' => { *r++ = '\v'; };
    '\\\\' => { *r++ = '\\'; };
    '\\?' => { *r++ = '\?'; };
    '\\\'' => { *r++ = '\''; };
    '\\"' => { *r++ = '\"'; };
    any => { *r++ = fc; };
  *|;
}%%

%% write data;
// clang-format on

static bool escapeSeqDecode(std::string_view input, std::string& result) {
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
    result.resize(r - result.data());
    return true;
  }

  return false;
}

// clang-format off
%%{
  machine escape_seq_decode_stream;
  
  action decode_hex {
    std::string decode;
    if(hexDecode({ts + 2, 2},decode) && !decode.empty()){
      result += decode.front();
    }
  }

  action decode_octal {
    unsigned char value = 0;
    const char* p_octal = ts + 1;

    // The first octal
    value = (*p_octal++ - '0');

    // The second octal
    if (p_octal < te) {
      value = (value << 3) | (*p_octal++ - '0'); 
    }

    // The third octal
    if (p_octal < te) {
      value = (value << 3) | (*p_octal++ - '0'); 
    }

    result += value;
  }

  hex = [0-9a-fA-F];
  octal = [0-7];
  main := |*
    '\\x' hex hex => decode_hex;
    '\\' octal octal? octal? => decode_octal;
    '\\a'  => { result += '\a'; };
    '\\b' => { result += '\b'; };
    '\\f' => { result += '\f'; };
    '\\n' => { result += '\n'; };
    '\\r' => { result += '\r'; };
    '\\t' => { result += '\t'; };
    '\\v' => { result += '\v'; };
    '\\\\' => { result += '\\'; };
    '\\?' => { result += '\?'; };
    '\\\'' => { result += '\''; };
    '\\"' => { result += '\"'; };
    any => { result += fc; };
  *|;
}%%

%% write data;
// clang-format on

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
escapeSeqDecodeNewStream() {
  return std::make_unique<Wge::Transformation::StreamState>();
}

static Wge::Transformation::StreamResult
escapeSeqDecodeStream(std::string_view input, std::string& result,
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

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}