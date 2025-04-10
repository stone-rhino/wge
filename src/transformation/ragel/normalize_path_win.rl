#pragma once

#include <string>
#include <string_view>
#include <normalize_path.h>

// Same as normalizePath, but first converts backslash characters to forward slashes.
%%{
  machine normalize_path_win;

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

  SLASH = '\\';

  # prescan
  main := |*
    SLASH => exec_transformation;
    any => skip;
  *|;

  transformation := |*
    SLASH => { *r++ = '/'; };
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

static bool normalizePathWin(std::string_view input, std::string& result2) {
  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r) {
    result.resize(r - result.data());
    bool ret = ::normalizePath(result, result2);
    if(!ret) {
      result2 = std::move(result);
    }

    return true;
  } else {
    return normalizePath(input, result2);
  }
}