#pragma once

#include <string>
#include <string_view>

%%{
  machine lowercase;
  
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

  # prescan
  main := |*
    [A-Z] => exec_transformation;
    any => skip;
  *|;
  
  transformation := |*
    [A-Z] => { *r++ = fc + 32; };
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

static bool lowercase(std::string_view input, std::string& result) {
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