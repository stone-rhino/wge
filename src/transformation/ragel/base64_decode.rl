#pragma once

#include <string>
#include <string_view>

%%{
  machine base64_decode;
  
  action decode_char {
    buffer = (buffer << 6) | base64_table[fc];
    count++;
    if (count == 4) {
      *r++ = (buffer >> 16) & 0xFF;
      *r++ = (buffer >> 8) & 0xFF;
      *r++ = buffer & 0xFF;
      buffer = 0;
      count = 0;
    }
  }

  main := |*
    [A-Za-z0-9+/] => decode_char;
    any => { result.clear(); fbreak; };
  *|;
}%%

%% write data;

static const char base64_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, // 32-47
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, // 48-63
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, // 64-79
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, // 80-95
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, // 96-111
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112-127
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 // 240-255
};

static bool base64Decode(std::string_view input, std::string& result) {
  result.clear();

  if(input.size() % 4 != 0) {
    return false; // Invalid base64 input
  }

  const char* p = input.data();
  const char* pe = p + input.size();

  // Remove the '=' padding
  for(int i = 0; pe > p && *(pe - 1) == '='; ++i) {
    --pe;

    // The max padding is 2
    if(i >= 2) {
      return false;
    }
  }

  result.resize(input.size() / 4 * 3);
  char* r = result.data();

  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  int buffer = 0;
  int count = 0;

  %% write init;
  %% write exec;

  if(!result.empty()) {
    // Process remaining bytes
    if(count == 2) {
      *r++ = (buffer >> 4) & 0xFF;
    } else if(count == 3) {
      *r++ = (buffer >> 10) & 0xFF;
      *r++ = (buffer >> 2) & 0xFF;
    }

    result.resize(r - result.data());
    return true;
  }

  return false;
}