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

#include "hex_decode.h"

%%{
  machine url_decode_uni;
  
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

  action decode_hex {
    std::string decode;
    if(hexDecode({ts + 1, 2},decode) && !decode.empty()){
      *r++ = decode.front();
    }
  }

  action decode_unicode {
    std::string decode;
    if(hexDecode({ts + 2, 4},decode) && decode.size() == 2){
      unsigned short value = static_cast<unsigned char>(decode[0]) << 8 | static_cast<unsigned char>(decode[1]);
  
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

  HEX = [0-9a-fA-F];

  # prescan
  main := |*
    '+' => exec_transformation;
    '%' HEX HEX => exec_transformation;
    '%' 'u' HEX HEX HEX HEX => exec_transformation;
    any => skip;
  *|;
  
  transformation := |*
    '+' => { *r++ = ' '; };
    '%' HEX HEX => decode_hex;
    '%' 'u' HEX HEX HEX HEX => decode_unicode;
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

static bool urlDecodeUni(std::string_view input, std::string& result) {
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