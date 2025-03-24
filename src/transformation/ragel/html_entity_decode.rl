#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

%%{
  machine html_entity_decode;

  action emit_entity {
    auto it = html_entities.find(entity_name);
    if (it != html_entities.end()) {
        result += it->second;
    } else {
        result += "&" + entity_name + ";";
    }

    fgoto main;
  }

  action emit_numeric_entity {
    try {
      if (is_hex) {
        result += static_cast<char>(std::stoi(entity_value, nullptr, 16));
      } else {
        result += static_cast<char>(std::stoi(entity_value));
      }
    } catch (...) {
      result += "&#" + entity_value + ";";
    }

    fgoto main;
  }

  main := |*
      '&' => { fgoto start_entity; };
      any => { result += fc; };
  *|;

  start_entity := |*
      [a-zA-Z] => { entity_name.clear(); entity_name += fc; fgoto entity_name; };
      '#' => { entity_value.clear(); fgoto start_numeric; };
      any => { result += '&'; fgoto main; };
  *|;

  entity_name := |*
      [a-zA-Z0-9] => { entity_name += fc; };
      ';' => emit_entity;
      any => { result += '&' + entity_name; fgoto main; };
  *|;

  start_numeric := |*
      'x' => { is_hex = true; fgoto numeric_value; };
      [0-9] => { is_hex = false; entity_value += fc; fgoto numeric_value; };
      any => { result += "&#"; fgoto main; };
  *|;

  numeric_value := |*
      [0-9a-fA-F] => { entity_value += fc; };
      ';' => emit_numeric_entity;
      any => { result += "&#" + entity_value; fgoto main; };
  *|;
}%%
  
std::string htmlEntityDecode(std::string_view input) {
  %% write data;
  static const std::unordered_map<std::string, char> html_entities = {
      {"amp", '&'},
      {"lt", '<'},
      {"gt", '>'},
      {"quot", '"'},
      {"apos", '\''},
      {"nbsp", ' '}
  };

  std::string result;
  std::string entity_name;
  std::string entity_value;
  bool is_hex = false;

  const char *p = input.data();
  const char *pe = input.data() + input.size();
  const char *eof = pe;

  int cs, act;
  const char* ts, *te;

  %% write init;
  %% write exec;

  return result;
}