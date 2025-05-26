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

// Decodes JavaScript escape sequences. If a \uHHHH code is in the range of FF01-FF5E (the full
// width ASCII codes), then the higher byte is used to detect and adjust the lower byte.
// Otherwise, only the lower byte will be used and the higher byte zeroed (leading to possible
// loss of information).
// And other decoding rule is similar to ANSI C escape sequences, Refer to escape_seq_decode.rl.  
%%{
  machine js_decode;
  
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

  action decode_unicode {
    std::string decode;
    if(hexDecode({ts + 2, 4},decode) && !decode.empty()) {
      unsigned short value = static_cast<unsigned char>(decode[0]) << 8 | static_cast<unsigned char>(decode[1]);

      // Convert to half-width ASCII
      if(value >= 0xFF01 && value <= 0xFF5E) {
        value -= 0xFEE0;
      }

      // Convert to Unicode
      if(value < 0x80) {
        // 1 byte
        *r++ = static_cast<char>(value);
      } else if(value < 0x800) {
        // 2 bytes
        *r++ = static_cast<char>(0xc0 | (value >> 6));
        *r++ = static_cast<char>(0x80 | (value & 0x3f));
      } else if(value < 0x10000) {
        // Surrogate pairs
        *r++ = static_cast<char>(0xe0 | (value >> 12));
        *r++ = static_cast<char>(0x80 | ((value >> 6) & 0x3f));
        *r++ = static_cast<char>(0x80 | (value & 0x3f));
      }
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
    '\\u' hex hex hex hex => decode_unicode;
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

static bool jsDecode(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r) {
    result.resize(r - result.data());
    return true;
  }

  return false;
}