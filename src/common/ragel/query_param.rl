#pragma once

#include <unordered_map>
#include <vector>
#include <string_view>

%%{
  machine query_param;

  action start_key {
    p_start_key = p;
  }

  action end_key {
    key_len = p - p_start_key;
  }

  action start_value {
    p_start_value = p;
  }

  action end_value {
    value_len = p - p_start_value;
  }

  action add_key_value {
    auto it = query_params.find({p_start_key,key_len});
    if (it != query_params.end()) {
      // If the key already exists, update the value
      it->second = std::string_view(p_start_value,value_len);
    } else {
      auto result = query_params.insert({std::string_view(p_start_key,key_len), std::string_view(p_start_value,value_len)});
      query_params_linked.emplace_back(result.first);
    }

    p_start_key = nullptr;
    p_start_value = nullptr;
    key_len = 0;
    value_len = 0;
  }

  key = [^&=]+ >start_key %end_key;
  value = [^&]+ >start_value %end_value;
  key_value = key ('=' value)? %add_key_value;
  main := 
    key_value ( '&' key_value )*;
}%%

%% write data;

static void parseQueryParam(std::string_view input,std::unordered_map<std::string_view, std::string_view>& query_params,
  std::vector<std::unordered_map<std::string_view, std::string_view>::iterator>& query_params_linked) {
  query_params.clear();
  query_params_linked.clear();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  int cs;

  const char* p_start_key = nullptr;
  const char* p_start_value = nullptr;
  size_t key_len = 0;
  size_t value_len = 0;

  %% write init;
  %% write exec;
}