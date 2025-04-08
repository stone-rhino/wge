#pragma once

#include "hex_decode.h"

// Decodes ANSI C escape sequences: \a, \b, \f, \n, \r, \t, \v, \\, \?, \', \", \xHH (hexadecimal), \0OOO (octal). 
// Invalid encodings are left in the output.
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
    std::string decode = hexDecode({ts + 2, 2});
    if(!decode.empty()){
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

std::string escapeSeqDecode(std::string_view input) {
  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r){
    result.resize(r - result.data());
  }
  return result;
}