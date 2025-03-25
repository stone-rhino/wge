#pragma once

#include <iostream>

%%{
  machine hex_decode;
  
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
    [0-9a-fA-F] => exec_transformation;
    any => { fbreak; };
  *|;

  transformation := |*
    '0' => { if((p - po) % 2 == 0){*r = 0 << 4;} else { *r++ |= 0; } };
    '1' => { if((p - po) % 2 == 0){*r = 1 << 4;} else { *r++ |= 1; } };
    '2' => { if((p - po) % 2 == 0){*r = 2 << 4;} else { *r++ |= 2; } };
    '3' => { if((p - po) % 2 == 0){*r = 3 << 4;} else { *r++ |= 3; } };
    '4' => { if((p - po) % 2 == 0){*r = 4 << 4;} else { *r++ |= 4; } };
    '5' => { if((p - po) % 2 == 0){*r = 5 << 4;} else { *r++ |= 5; } };
    '6' => { if((p - po) % 2 == 0){*r = 6 << 4;} else { *r++ |= 6; } };
    '7' => { if((p - po) % 2 == 0){*r = 7 << 4;} else { *r++ |= 7; } };
    '8' => { if((p - po) % 2 == 0){*r = 8 << 4;} else { *r++ |= 8; } };
    '9' => { if((p - po) % 2 == 0){*r = 9 << 4;} else { *r++ |= 9; } };
    [aA] => { if((p - po) % 2 == 0){*r = 10 << 4;} else { *r++ |= 10; } };
    [bB] => { if((p - po) % 2 == 0){*r = 11 << 4;} else { *r++ |= 11; } };
    [cC] => { if((p - po) % 2 == 0){*r = 12 << 4;} else { *r++ |= 12; } };
    [dD] => { if((p - po) % 2 == 0){*r = 13 << 4;} else { *r++ |= 13; } };
    [eE] => { if((p - po) % 2 == 0){*r = 14 << 4;} else { *r++ |= 14; } };
    [fF] => { if((p - po) % 2 == 0){*r = 15 << 4;} else { *r++ |= 15; } };
    any => { fbreak; };
  *|;

}%%

%% write data;

std::string hexDecode(std::string_view input) {
  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* po = p;
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