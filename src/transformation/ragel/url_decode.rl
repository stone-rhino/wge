#pragma once

#include <string>
#include <string_view>

#include "hex_decode.h"

// Removes all whitespace characters from input.
%%{
  machine remove_nulls;
  
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

  HEX = [0-9a-fA-F];

  # prescan
  main := |*
    '+' => exec_transformation;
    '%' HEX HEX => exec_transformation;
    any => skip;
  *|;
  
  transformation := |*
    '+' => { *r++ = ' '; };
    '%' HEX HEX => decode_hex;
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

static bool urlDecode(std::string_view input, std::string& result) {
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