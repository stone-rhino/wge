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

#include <cstring>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "src/transformation/stream_util.h"

#ifndef ENABLE_HTML_ENTITY_DECODE_DEBUG_LOG
#define ENABLE_HTML_ENTITY_DECODE_DEBUG_LOG 0
#endif

#if ENABLE_HTML_ENTITY_DECODE_DEBUG_LOG
#include <format>
#include <iostream>
#define HTML_ENTITY_DECODE_LOG(x) std::cout << x << std::endl;
#else
#define HTML_ENTITY_DECODE_LOG(x)
#endif

// clang-format off
%%{
  machine html_entity_decode;

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

  action exec_transformation_if_eof {
    if(te == eof) {
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
  }

  #prescan 
  main := |*
    '&amp;' => exec_transformation;
    '&lt;' => exec_transformation;
    '&gt;' => exec_transformation;
    '&quot;' => exec_transformation;
    '&apos;' => exec_transformation;
    '&nbsp;' => exec_transformation;
    '&#' '0'* [0-9]{1,7} [^0-9a-fA-F] => exec_transformation;
    '&#' [xX] '0'* [0-9a-fA-F]{1,6} [^0-9a-fA-F] => exec_transformation;
    '&#' '0'* [0-9]{1,7} => exec_transformation_if_eof;
    '&#' [xX] '0'* [0-9a-fA-F]{1,6} => exec_transformation_if_eof;
    any => {};
  *|;

  transformation := |*
    '&amp;' => { *r++ = '&';};
    '&lt;' => { *r++ = '<';};
    '&gt;' => { *r++ = '>';};
    '&quot;' => { *r++ = '"';};
    '&apos;' => { *r++ = '\'';};
    '&nbsp;' => { *r++ = ' ';};
    '&#' '0'* [0-9]{1,7} [^0-9a-fA-F] => {
      is_hex = false;
      entity_value = std::string(ts + 2, te - ts - 3);
      emitNumericEntity(&r, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }
    };
    '&#' [xX] '0'* [0-9a-fA-F]{1,6} [^0-9a-fA-F] => {
      is_hex = true;
      entity_value = std::string(ts + 3, te - ts - 4);
      emitNumericEntity(&r, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }
    };
    '&#' '0'* [0-9]{1,7} => {
      if( te == eof ) {
        is_hex = false;
        entity_value = std::string(ts + 2, te - ts - 2);
        emitNumericEntity(&r, entity_value, is_hex);
      }
    };
    '&#' [xX] '0'* [0-9a-fA-F]{1,6} => {
      if( te == eof ) {
        is_hex = true;
        entity_value = std::string(ts + 3, te - ts - 3);
        emitNumericEntity(&r, entity_value, is_hex);
      }
    };
    any => { *r++ = fc; };
  *|;
}%%
  
%% write data;
// clang-format on

static void emitNumericEntity(char** r, const std::string& entity_value, bool is_hex) {
  try {
    if (is_hex) {
      **r = static_cast<char>(std::stoi(entity_value, nullptr, 16));
      (*r)++;
    } else {
      **r = static_cast<char>(std::stoi(entity_value));
      (*r)++;
    }
  } catch (...) {
    memcpy(*r, "&#", 2);
    *r += 2;
    memcpy(*r, entity_value.data(), entity_value.size());
    *r += entity_value.size();
    **r = ';';
    (*r)++;
  }
}

static bool htmlEntityDecode(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

  std::string entity_name;
  std::string entity_value;
  bool is_hex = false;

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
  machine html_entity_decode_stream;

  main := |*
    '&amp;' => {
      result += '&';
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &amp;"));
    };
    '&lt;' => {
      result += '<'; 
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &lt;"));
    };
    '&gt;' => {
      result += '>';
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &gt;"));
    };
    '&quot;' => {
      result += '"';
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &quot;"));
    };
    '&apos;' => {
      result += '\'';
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &apos;"));
    };
    '&nbsp;' => {
      result += ' ';
      HTML_ENTITY_DECODE_LOG(std::format("Decoded entity: &nbsp;"));
    };
    '&#' '0' => {
      skip_prefix_zero_d_count = 1;
      HTML_ENTITY_DECODE_LOG(std::format("Found numeric entity with decimal prefix"));
      fgoto skip_prefix_zero_d;
    };
    '&#' [xX] '0' {
      skip_prefix_zero_h_count = 1;
      hex_prefix = *(ts + 2);
      HTML_ENTITY_DECODE_LOG(std::format("Found numeric entity with hexadecimal prefix: {}", hex_prefix));
      fgoto skip_prefix_zero_h;
    };
    '&#' [1-9] [0-9]{1,6} [^0-9a-fA-F] => {
      bool is_hex = false;
      std::string entity_value(ts + 2, te - ts - 3);
      HTML_ENTITY_DECODE_LOG(std::format("Decoded numeric entity: &#{};", entity_value));
      emitNumericEntity(result, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }
    };
    '&#' [xX] [1-9a-fA-F] [0-9a-fA-F]{1,5} [^0-9a-fA-F] => {
      bool is_hex = true;
      std::string entity_value(ts + 3, te - ts - 4);
      HTML_ENTITY_DECODE_LOG(std::format("Decoded numeric entity: &#x{};", entity_value));
      emitNumericEntity(result, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }

    };
    '&#' [1-9] [0-9]{1,6} => {
      if( te == eof ) {
        bool is_hex = false;
        std::string entity_value(ts + 2, te - ts - 2);
        HTML_ENTITY_DECODE_LOG(std::format("Decoded numeric entity at eof: &#{};", entity_value));
        emitNumericEntity(result, entity_value, is_hex);
      } else {
        HTML_ENTITY_DECODE_LOG(std::format("Appending decimal numeric entity: {}", ts + 2));
        result.append(ts, te - ts);
      }
    };
    '&#' [xX] [1-9a-fA-F] [0-9a-fA-F]{1,5} => {
      if( te == eof ) {
        bool is_hex = true;
        std::string entity_value(ts + 3, te - ts - 3);
        HTML_ENTITY_DECODE_LOG(std::format("Decoded numeric entity at eof: &#x{};", entity_value));
        emitNumericEntity(result, entity_value, is_hex);
      } else {
        HTML_ENTITY_DECODE_LOG(std::format("Appending hexadecimal numeric entity: {}", ts + 3));
        result.append(ts, te - ts);
      }
    };
    any => {
      result += fc;
      HTML_ENTITY_DECODE_LOG(std::format("Appending character: {}", fc));
    };
  *|;

  skip_prefix_zero_d := |*
    '0' => {
      skip_prefix_zero_d_count++;
      HTML_ENTITY_DECODE_LOG(std::format("Skipping zero in decimal numeric entity, count: {}", skip_prefix_zero_d_count));
    };
    any => {
      HTML_ENTITY_DECODE_LOG(std::format("Found non-zero character in skipped decimal numeric entity: {}", fc));
      p = ts;
      fhold;
      fgoto number_entity_d;
    };
  *|;

  skip_prefix_zero_h := |*
    '0' => {
      skip_prefix_zero_h_count++;
      HTML_ENTITY_DECODE_LOG(std::format("Skipping zero in hexadecimal numeric entity, count: {}", skip_prefix_zero_h_count));
    };
    any => {
      HTML_ENTITY_DECODE_LOG(std::format("Found non-zero character in skipped hexadecimal numeric entity: {}", fc));
      p = ts;
      fhold;
      fgoto number_entity_h;
    };
  *|;

  number_entity_d := |*
    [0-9]{1,7} [^0-9a-fA-F] => {
      bool is_hex = false;
      std::string entity_value(ts, te - ts - 1);
      HTML_ENTITY_DECODE_LOG(std::format("Decoded decimal numeric entity: {}", entity_value));
      emitNumericEntity(result, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }
      fgoto main;
    };
    [0-9]{1,7} => {
      if( te == eof ) {
        bool is_hex = false;
        std::string entity_value(ts, te - ts);
        HTML_ENTITY_DECODE_LOG(std::format("Decoded decimal numeric entity at eof: {}", entity_value));
        emitNumericEntity(result, entity_value, is_hex);
      } else {
        result += "&#";
        result.append(skip_prefix_zero_d_count, '0');
        result.append(ts, te - ts);
        HTML_ENTITY_DECODE_LOG(std::format("Appending decimal numeric entity: &#0({}){}", skip_prefix_zero_d_count, std::string_view(ts, te - ts)));
      }
      fgoto main;
    };
    any => {
      HTML_ENTITY_DECODE_LOG(std::format("Found non-numeric character in decimal numeric entity: {}", fc));
      result += "&#";
      result.append(skip_prefix_zero_d_count, '0');
      p = ts;
      fhold;
      fgoto main;
    };
  *|;

  number_entity_h := |*
    [0-9a-fA-F]{1,6} [^0-9a-fA-F] => {
      bool is_hex = true;
      std::string entity_value(ts, te - ts);
      HTML_ENTITY_DECODE_LOG(std::format("Decoded hexadecimal numeric entity: {}", entity_value));
      emitNumericEntity(result, entity_value, is_hex);
      if(fc != ';') {
        fhold;
      }
      fgoto main;
    };
    [0-9a-fA-F]{1,6} => {
      if( te == eof ) {
        bool is_hex = true;
        std::string entity_value(ts, te - ts);
        HTML_ENTITY_DECODE_LOG(std::format("Decoded hexadecimal numeric entity at eof: {}", entity_value));
        emitNumericEntity(result, entity_value, is_hex);
      } else {
        result += "&#";
        result += hex_prefix;
        result.append(skip_prefix_zero_h_count, '0');
        result.append(ts, te - ts);
        HTML_ENTITY_DECODE_LOG(std::format("Appending hexadecimal numeric entity: &#{}0({}){}", hex_prefix, skip_prefix_zero_h_count, std::string_view(ts, te - ts)));
      }
      fgoto main;
    };
    any => {
      HTML_ENTITY_DECODE_LOG(std::format("Found non-hexadecimal character in hexadecimal numeric entity: {}", fc));
      result += "&#";
      result += hex_prefix;
      result.append(skip_prefix_zero_h_count, '0');
      p = ts;
      fhold;
      fgoto main;
    };
  *|;
}%%
  
%% write data;
// clang-format on

static void emitNumericEntity(std::string& result, const std::string& entity_value, bool is_hex) {
  try {
    if (is_hex) {
      result += static_cast<char>(std::stoi(entity_value, nullptr, 16));
    } else {
      result += static_cast<char>(std::stoi(entity_value));
    }
  } catch (...) {
    result += "&#";
    result += entity_value;
    result += ';';
  }
}

struct HtmlEntityDecodeExtraState {
  size_t skip_prefix_zero_d_count_{0};
  size_t skip_prefix_zero_h_count_{0};
  char hex_prefix_{'x'};
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
htmlEntityDecodeNewStream() {
  return Wge::Transformation::newStreamWithExtraState<HtmlEntityDecodeExtraState>();
}

static Wge::Transformation::StreamResult
htmlEntityDecodeStream(std::string_view input, std::string& result,
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

  HtmlEntityDecodeExtraState* extra_state =
      reinterpret_cast<HtmlEntityDecodeExtraState*>(state.extra_state_buffer_.data());
  size_t& skip_prefix_zero_d_count = extra_state->skip_prefix_zero_d_count_;
  size_t& skip_prefix_zero_h_count = extra_state->skip_prefix_zero_h_count_;
  char& hex_prefix = extra_state->hex_prefix_;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}