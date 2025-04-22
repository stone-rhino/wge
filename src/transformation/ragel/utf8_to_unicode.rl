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
    any => { r = nullptr; fbreak; };
  *|;
}%%

%% write data;

static constexpr std::string_view table{"0123456789abcdef"};

static bool utf8ToUnicode(std::string_view input, std::string& result) {
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
    return true;
  }

  return false;
}