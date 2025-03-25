#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <iostream>

%%{
  machine html_entity_decode;

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

  #prescan 
  main := |*
    '&amp;' => exec_transformation;
    '&lt;' => exec_transformation;
    '&gt;' => exec_transformation;
    '&quot;' => exec_transformation;
    '&apos;' => exec_transformation;
    '&nbsp;' => exec_transformation;
    '&#' [0-9]+ ';' => exec_transformation;
    '&#x' [0-9a-fA-F]+ ';' => exec_transformation;
    any => {};
  *|;

  transformation := |*
    '&amp;' => { *r++ = '&';};
    '&lt;' => { *r++ = '<';};
    '&gt;' => { *r++ = '>';};
    '&quot;' => { *r++ = '"';};
    '&apos;' => { *r++ = '\'';};
    '&nbsp;' => { *r++ = ' ';};
    '&#' [0-9]+ ';' => {
      is_hex = false;
      entity_value = std::string(ts + 2, te - ts - 3);
      emitNumericEntity(&r, entity_value, is_hex);
    };
    '&#x' [0-9a-fA-F]+ ';' => {
      is_hex = true;
      entity_value = std::string(ts + 3, te - ts - 4);
      emitNumericEntity(&r, entity_value, is_hex);
    };
    any => { *r++ = *p; };
  *|;
}%%
  
%% write data;

void emitNumericEntity(char** r, const std::string& entity_value, bool is_hex) {
  try {
    if (is_hex) {
      **r = static_cast<char>(std::stoi(entity_value, nullptr, 16));
      (*r)++;
    } else {
      **r = static_cast<char>(std::stoi(entity_value));
      (*r)++;
    }
  } catch (...) {
    memcpy(*r, "&#", 2);
    *r += 2;
    memcpy(r, entity_value.data(), entity_value.size());
    *r += entity_value.size();
    **r = ';';
    (*r)++;
  }
}

std::string htmlEntityDecode(std::string_view input) {
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