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

#include <url_decode.h>

// clang-format off
%%{
  machine uri_parser;

  action start_path {
    p_start_path = p;
  }

  action end_path {
    relative_uri_len = p - input.data();
    uri_len = p - p_start_path;
  }

  action start_base_name {
    p_start_base_name = p;
  }


  action end_base_name {
    base_name = std::string_view(p_start_base_name, p - p_start_base_name);
    relative_uri_len = p - input.data();
    uri_len = p - p_start_path;
  }

  action start_query {
    p_start_query = p;
  }

  action end_query {
    query = std::string_view(p_start_query, p - p_start_query);
    uri_len = p - p_start_path;
  }

  host = ("http://" | "https://") [^/]+;
  path = '/' >start_path ([^/?#]* '/')*  %end_path;
  base_name = [^/?#]+ >start_base_name %end_base_name;
  query = [^#]+ >start_query %end_query;
  main := 
    host? path base_name? ('?' query)? ('#' any+)?;
}%%

%% write data;
// clang-format on

static void uriParser(std::string_view input, std::string_view& uri, std::string_view& relative_uri,
                      std::string_view& query, std::string_view& base_name,
                      std::forward_list<std::string>& parser_buffer) {

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  int cs;

  const char* p_start_path = nullptr;
  const char* p_start_base_name = nullptr;
  const char* p_start_query = nullptr;
  size_t relative_uri_len = 0;
  size_t uri_len = 0;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  uri = std::string_view(p_start_path, uri_len);
  relative_uri = std::string_view(input.data(), relative_uri_len);
  std::string base_name_buffer;
  std::string relative_uri_buffer;
  std::string uri_buffer;
  if (!base_name.empty() && urlDecode(base_name, base_name_buffer, false))
    [[unlikely]] { base_name = parser_buffer.emplace_front(std::move(base_name_buffer)); }
  if (urlDecode(relative_uri, relative_uri_buffer, false))
    [[unlikely]] { relative_uri = parser_buffer.emplace_front(std::move(relative_uri_buffer)); }
  if (urlDecode(uri, uri_buffer, false)) {
    uri = parser_buffer.emplace_front(std::move(uri_buffer));
  }
}