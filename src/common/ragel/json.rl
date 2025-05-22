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

#include <string_view>
#include <vector>
#include <unordered_map>
#include <escape_seq_decode.h>
#include <forward_list>

#ifndef ENABLE_JSON_DEBUG_LOG
#define ENABLE_JSON_DEBUG_LOG 0
#endif

#if ENABLE_JSON_DEBUG_LOG
#include <iostream>
#include <format>
#define JSON_LOG(x) std::cout << x << std::endl;
#else
#define JSON_LOG(x)
#endif

// Ragel state machine for parsing JSON objects.
// 1. All of the key-value pairs are stored in a flat map/vector. We don't care about the structure
// of the JSON object.
// 2. The object, boolean, null, number are only have key, the value is an empty string.
// 3. If the elements of the array are objects, There is only one key of the array in the flat
// map/vector, and the value is an empty string. But the objects of the array are stored in the flat
// map/vector with key-value pairs.
// 4. If the elements of the array are boolean, null, number, There is only one key of the array in
// the flat map/vector, and the value is an empty string.
// 5. If the elements of the array are strings, there are multiple string values in the flat
// map/vector with the same key.
%%{
    machine json;

    action skip {}

    action error {
      JSON_LOG(std::format("error: {}", std::string_view(ts, 10)));
      error = true;
      fbreak;
    }

    action add_key {
      key_view = std::string_view(ts + 1, te - ts - 3);
      std::string key_escape_buffer;
      if(escapeSeqDecode(key_view, key_escape_buffer)) {
        escape_buffer.emplace_front(std::move(key_escape_buffer));
        key_view = escape_buffer.front();
      }
      value_view = {};
      JSON_LOG(std::format("add_key: {}", key_view));
      JSON_LOG("fcall value");
      fcall value;
    }

    action add_string_value {
      value_view = std::string_view(ts + 1, te - ts - 2);
      std::string value_escape_buffer;
      if(escapeSeqDecode(value_view, value_escape_buffer)) {
        escape_buffer.emplace_front(std::move(value_escape_buffer));
        value_view = escape_buffer.front();
      }
      key_value_map.insert({key_view, value_view});
      key_value_linked.emplace_back(key_view, value_view);
      JSON_LOG(std::format("add_string_value: {}", value_view));
    }
  
    action skip_object_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_object_value: {}", key_view));
      JSON_LOG("fret value");
      fret;
    }

    action skip_number_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_number_value: {}", std::string_view(ts, te - ts)));
    }

    action skip_boolean_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_boolean_value: {}", std::string_view(ts, te - ts)));
    }

    action skip_null_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_null_value: {}", std::string_view(ts, te - ts)));
    }

    #####################################################

    action skip_array_object_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      square_bracket_count = 0;
      JSON_LOG(std::format("skip_array_object_value: {}", key_view));
      JSON_LOG("fgoto main");
      fgoto main;
    }

    action skip_array_number_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_array_number_value: {}", std::string_view(ts, te - ts)));
      JSON_LOG("fnext skip_array_elemt");
      fnext skip_array_elemt;
    }

    action skip_array_boolean_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_array_boolean_value: {}", std::string_view(ts, te - ts)));
      JSON_LOG("fnext skip_array_elemt");
      fnext skip_array_elemt;
    }

    action skip_array_null_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_array_null_value: {}", std::string_view(ts, te - ts)));
      JSON_LOG("fnext skip_array_elemt");
      fnext skip_array_elemt;
    }

    WS = [ \t\r\n]*;
    
    main := |*
      WS => skip;
      '{' => skip;
      '}' => skip;
      ',' => skip;
      ']' => skip;
      '"' ([^"] | ('\\"'))* '":' => add_key;
      any => error;
    *|;

    value := |*
      WS => skip;
      '[' => { 
        JSON_LOG("fcall array");
        ++square_bracket_count;
        fcall array;
      };
      '}' | ',' => {
        JSON_LOG("fret value");
        fret;
      };
      '{' => skip_object_value;
      'true' => skip_boolean_value;
      'false' => skip_boolean_value;
      'null' => skip_null_value;
      '-'? [0-9]+ '.'? [0-9]* => skip_number_value;
      '"' ([^"] | ('\\"'))* '"' => add_string_value;
      any => error;
    *|;

    array := |*
      WS => skip;
      '[' => {
        JSON_LOG("array nesting open");
        ++square_bracket_count;
      };
      ']' => {
        JSON_LOG("array nesting close");
        --square_bracket_count;
        if (square_bracket_count == 0) {
          JSON_LOG("fret array");
          fret;
        } 
      };
      '{' => skip_array_object_value;
      '}' => skip;
      ',' => skip;
      'true' => skip_array_boolean_value;
      'false' => skip_array_boolean_value;
      'null' => skip_array_null_value;
      '-'? [0-9]+ '.'? [0-9]* => skip_array_number_value;
      '"' ([^"] | ('\\"'))* '"' => add_string_value;
      any => error;
    *|;

    skip_array_elemt := |*
      '[' => {
        JSON_LOG("array nesting open");
        ++square_bracket_count;
      };
      ']' => {
        JSON_LOG("array nesting close");
        --square_bracket_count;
        if (square_bracket_count == 0) {
          JSON_LOG("fret array");
          fret;
        } 
      };
      any => skip;
    *|;
}%%

%% write data;

static bool parseJson(std::string_view input, 
    std::unordered_multimap<std::string_view, std::string_view>& key_value_map, 
    std::vector<std::pair<std::string_view, std::string_view>>& key_value_linked,
    std::forward_list<std::string>& escape_buffer) {
  key_value_map.clear();
  key_value_linked.clear();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;
  int top = 0;
  int stack[16];

  std::string_view key_view;
  std::string_view value_view;
  bool error = false;

  // For supporting infinite nested array, we don't use a stack to save the state,
  // but only use the number of square brackets to determine the level of nesting
  size_t square_bracket_count = 0;


  %% write init;
  %% write exec;

  return error;
}

#undef JSON_LOG