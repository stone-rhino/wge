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
    '0' => { if((p - po) % 2 == 0){*r++ = 0 << 4;} else { *(r-1) |= 0; } };
    '1' => { if((p - po) % 2 == 0){*r++ = 1 << 4;} else { *(r-1) |= 1; } };
    '2' => { if((p - po) % 2 == 0){*r++ = 2 << 4;} else { *(r-1) |= 2; } };
    '3' => { if((p - po) % 2 == 0){*r++ = 3 << 4;} else { *(r-1) |= 3; } };
    '4' => { if((p - po) % 2 == 0){*r++ = 4 << 4;} else { *(r-1) |= 4; } };
    '5' => { if((p - po) % 2 == 0){*r++ = 5 << 4;} else { *(r-1) |= 5; } };
    '6' => { if((p - po) % 2 == 0){*r++ = 6 << 4;} else { *(r-1) |= 6; } };
    '7' => { if((p - po) % 2 == 0){*r++ = 7 << 4;} else { *(r-1) |= 7; } };
    '8' => { if((p - po) % 2 == 0){*r++ = 8 << 4;} else { *(r-1) |= 8; } };
    '9' => { if((p - po) % 2 == 0){*r++ = 9 << 4;} else { *(r-1) |= 9; } };
    [aA] => { if((p - po) % 2 == 0){*r++ = 10 << 4;} else { *(r-1) |= 10; } };
    [bB] => { if((p - po) % 2 == 0){*r++ = 11 << 4;} else { *(r-1) |= 11; } };
    [cC] => { if((p - po) % 2 == 0){*r++ = 12 << 4;} else { *(r-1) |= 12; } };
    [dD] => { if((p - po) % 2 == 0){*r++ = 13 << 4;} else { *(r-1) |= 13; } };
    [eE] => { if((p - po) % 2 == 0){*r++ = 14 << 4;} else { *(r-1) |= 14; } };
    [fF] => { if((p - po) % 2 == 0){*r++ = 15 << 4;} else { *(r-1) |= 15; } };
    any => { fhold; fbreak; };
  *|;

}%%

%% write data;

static bool hexDecode(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* po = p;
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r) {
    result.resize(r - result.data());

    // If the input length is odd, we assume the last character is a low 4 bits of a byte
    if((p - po) % 2 != 0){
      result.back() = result.back() >> 4 & 0x0F;
    }
    return true;
  }

  return false;
}