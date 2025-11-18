/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <cstring>
#include <forward_list>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <url_decode.h>

// clang-format off
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
    if (key_len != 0 || value_len != 0) [[likely]] {
      std::string_view raw_key(p_start_key, key_len), raw_value(p_start_value, value_len);
      std::string decoded_key_str, decoded_value_str;
      std::string_view final_key = raw_key;
      std::string_view final_value = raw_value;
      if (urlDecode(raw_key, decoded_key_str)) {
        urldecoded_storage.emplace_front(std::move(decoded_key_str));
        final_key = urldecoded_storage.front();
      }
      if (urlDecode(raw_value, decoded_value_str)) {
        urldecoded_storage.emplace_front(std::move(decoded_value_str));
        final_value = urldecoded_storage.front();
      }
      auto result = query_params.insert({final_key, final_value});
      query_params_linked.emplace_back(final_key, final_value);

      p_start_key = nullptr;
      p_start_value = nullptr;
      key_len = 0;
      value_len = 0;
    }
  }

  key = [^&=]* >start_key %end_key;
  value = [^&]* >start_value %end_value;
  key_value = key ('=' value)? %add_key_value;
  main := 
    key_value ( '&' key_value )*;
}%%

%% write data;
// clang-format on

static void
parseQueryParam(std::string_view input,
                std::unordered_multimap<std::string_view, std::string_view>& query_params,
                std::vector<std::pair<std::string_view, std::string_view>>& query_params_linked,
                std::forward_list<std::string>& urldecoded_storage) {
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

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on
}
