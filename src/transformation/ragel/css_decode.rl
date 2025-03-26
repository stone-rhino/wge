#pragma once

#include "hex_decode.h"

%%{
  machine css_decode;
  
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

  hex = [0-9a-fA-F];
  not_hex = [^0-9a-fA-F\n];

  # prescan
  main := |*
    '\\' => exec_transformation;
    any => {};
  *|;

  transformation := |*
    '\\' hex{1,6} ' ' any => { entity_value = std::string(ts + 1, te - ts - 3); emitNumericEntity(&r, entity_value); *r++ = fc; };
    '\\' hex{1,6} => { entity_value = std::string(ts + 1, te - ts - 1); emitNumericEntity(&r, entity_value); };
    '\\' not_hex => { *r++ = fc; };
    '\\' '\n' => {}; # A newline character following backslash is ignored
    '\\' => {}; # The backslash is at the end of the input
    any => { *r++ = fc; };
  *|;
}%%

%% write data;

void emitNumericEntity(char** r, const std::string& entity_value) {
  std::string decoded;
  std::string_view data(entity_value);

  // If length of entity_value is greater than 2, then use the last two from the end
  if(entity_value.size() > 2){
    data = {entity_value.data() + entity_value.size() - 2, 2};
  }

  // If the length of entity_value is greater than 3, and the value of entity_value 
  // is like "FFxx","0FFxx","00FFxx", we need do full width conversion
  bool full_width = false;
  if(entity_value.size() > 3){
    switch(entity_value.size()){
      case 4:
        full_width = (entity_value[0] == 'f' || entity_value[0] == 'F') && (entity_value[1] == 'f' || entity_value[1] == 'F');
        break;
      case 5:
        full_width = entity_value[0] == '0' && (entity_value[1] == 'f' || entity_value[1] == 'F') && (entity_value[2] == 'f' || entity_value[2] == 'F');
        break;
      case 6:
        full_width = entity_value[0] == '0' && entity_value[1] == '0' && (entity_value[1] == 'f' || entity_value[1] == 'F') && (entity_value[2] == 'f' || entity_value[2] == 'F');
        break;
      default:
        assert(false);
        break;
    }
  }
 
  decoded = hexDecode(data);
  if(full_width){
    decoded.front() += 0x20;
  }
  memcpy(*r, decoded.data(), decoded.size());
  *r += decoded.size();
}

std::string cssDecode(std::string_view input) {
  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  std::string entity_name;
  std::string entity_value;
  bool is_hex = false;

  %% write init;
  %% write exec;

  if(r){
    result.resize(r - result.data());
  }
  return result;
}