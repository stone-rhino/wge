#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

%%{
  machine html_entity_decode;

  main := |*
    '&amp;' => {
      result += "&";
    };
    '&lt;' => {
      result += "<";
    };
    '&gt;' => {
      result += ">";
    };
    '&quot;' => {
      result += "\"";
    };
    '&apos;' => {
      result += "'";
    };
    '&nbsp;' => {
      result += " ";
    };
    '&' [a-zA-Z]+ ';' => {
      result.append(ts, te - ts);
    };
    '&#' [0-9]+ ';' => {
      is_hex = false;
      entity_value = std::string(ts + 2, te - ts - 3);
      emitNumericEntity(result, entity_value, is_hex);
    };
    '&#x' [0-9a-fA-F]+ ';' => {
      is_hex = true;
      entity_value = std::string(ts + 3, te - ts - 4);
      emitNumericEntity(result, entity_value, is_hex);
    };
    any => { result += *p; };
  *|;
}%%
  
%% write data;

void emitNumericEntity(std::string& result, const std::string& entity_value, bool is_hex) {
  try {
    if (is_hex) {
        result += static_cast<char>(std::stoi(entity_value, nullptr, 16));
    } else {
        result += static_cast<char>(std::stoi(entity_value));
    }
  } catch (...) {
    result += "&#" + entity_value + ";";
  }
}

std::string htmlEntityDecode(std::string_view input) {
    std::string result;
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

    return result;
}