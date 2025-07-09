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

#include <forward_list>
#include <list>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <js_decode.h>

#include "src/common/ragel/common.h"
#include "src/transformation/stream_util.h"

#ifndef ENABLE_JSON_DEBUG_LOG
#define ENABLE_JSON_DEBUG_LOG 0
#endif

#if ENABLE_JSON_DEBUG_LOG
#include <format>
#include <iostream>
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
// 4. If the elements of the array are boolean, null, number, there is only one key of the array in
// the flat map/vector, and the value is an empty string.
// 5. If the elements of the array are strings, there is only one key of the array in the flat
// map/vector, and the value is an empty string and there are multiple string values in the flat
// map/vector with an empty key.
// clang-format off
%%{
    machine json;

    action skip {}

    action error {
      JSON_LOG(std::format("error: {}", std::string_view(ts, 10)));
      error = true;
      fbreak;
    }

    action find_key {
      key_view = std::string_view(ts + 1, te - ts - 2);
      key_view = trimRight(key_view.data(), key_view.size());
      key_view.remove_suffix(1);
      std::string key_escape_buffer;
      if(jsDecode(key_view, key_escape_buffer)) {
        escape_buffer.emplace_front(std::move(key_escape_buffer));
        key_view = escape_buffer.front();
      }
      value_view = {};
      JSON_LOG(std::format("find_key: {}", key_view));
      JSON_LOG("fcall value");
      fcall value;
    }

    action add_string_value {
      value_view = std::string_view(ts + 1, te - ts - 2);
      std::string value_escape_buffer;
      if(jsDecode(value_view, value_escape_buffer)) {
        escape_buffer.emplace_front(std::move(value_escape_buffer));
        value_view = escape_buffer.front();
      }
      key_value_map.insert({key_view, value_view});
      key_value_linked.emplace_back(key_view, value_view);
      JSON_LOG(std::format("add_string_value. insert key-value: {}: {}", key_view, value_view));
    }
  
    action skip_object_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_object_value. insert key-value: {}: {}", key_view,""));
      JSON_LOG("fret value");
      fret;
    }

    action skip_number_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_number_value: {} insert key-value: {}: {}", std::string_view(ts, te - ts), key_view, ""));
    }

    action skip_boolean_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_boolean_value: {} insert key-value: {}: {}", std::string_view(ts, te - ts), key_view, ""));
    }

    action skip_null_value {
      key_value_map.insert({key_view, {}});
      key_value_linked.emplace_back(key_view, "");
      JSON_LOG(std::format("skip_null_value: {} insert key-value: {}: {}", std::string_view(ts, te - ts), key_view, ""));
    }

    #####################################################

    action add_array_string_value {
      value_view = std::string_view(ts + 1, te - ts - 2);
      std::string value_escape_buffer;
      if(jsDecode(value_view, value_escape_buffer)) {
        escape_buffer.emplace_front(std::move(value_escape_buffer));
        value_view = escape_buffer.front();
      }
      key_value_map.insert({"", value_view});
      key_value_linked.emplace_back("", value_view);
      JSON_LOG(std::format("add_array_string_value. insert key-value: {}: {}", key_view, value_view));
    }

    action skip_array_object_value {
      square_bracket_count = 0;
      JSON_LOG(std::format("skip_array_object_value"));
      JSON_LOG("fret array");
      fret;
    }

    action skip_array_number_value {
      JSON_LOG(std::format("skip_array_number_value: {}", std::string_view(ts, te - ts)));
    }

    action skip_array_boolean_value {
      JSON_LOG(std::format("skip_array_boolean_value: {}", std::string_view(ts, te - ts)));
    }

    action skip_array_null_value {
      JSON_LOG(std::format("skip_array_null_value: {}", std::string_view(ts, te - ts)));
    }

    WS = [ \t\r\n]*;
    
    main := |*
      WS => skip;
      '{' => skip;
      '}' => skip;
      ',' => skip;
      '[' => {
        JSON_LOG("start with array. fcall value");
        p = ts;
        fhold;
        fcall value;
      };
      ']' => skip;
      '"' ([^"] | ('\\"'))* '"' WS ':' => find_key;
      any => error;
    *|;

    value := |*
      WS => skip;
      '[' => {
        key_value_map.insert({key_view, {}});
        key_value_linked.emplace_back(key_view, "");
        JSON_LOG(std::format("fnext array. insert key-value: {}: {}", key_view, ""));
        ++square_bracket_count;
        fnext array;
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
      '"' ([^"] | ('\\"'))* '"' => add_array_string_value;
      any => error;
    *|;
}%%

%% write data;
// clang-format on

// Trims trailing whitespace
static std::string_view trimRight(const char* start, size_t size) {
  const char* end = start + size;

  // Trim trailing whitespace
  while (end > start &&
         (*(end - 1) == ' ' || *(end - 1) == '\t' || *(end - 1) == '\r' || *(end - 1) == '\n')) {
    --end;
  }

  return std::string_view(start, end - start);
}

static bool parseJson(std::string_view input,
                      std::unordered_multimap<std::string_view, std::string_view>& key_value_map,
                      std::vector<std::pair<std::string_view, std::string_view>>& key_value_linked,
                      std::forward_list<std::string>& escape_buffer) {
  key_value_map.clear();
  key_value_linked.clear();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;
  int top = 0;
  int stack[16];

  std::string_view key_view;
  std::string_view value_view;
  bool error = false;

  // For supporting infinite nested array, we don't use a stack to save the state,
  // but only use the number of square brackets to determine the level of nesting
  size_t square_bracket_count = 0;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  return error;
}

// clang-format off
%%{
    machine json_stream;

    action skip {}

    action error {
      JSON_LOG(std::format("error: {}", std::string_view(ts, 10)));
      error = true;
      fbreak;
    }

    action skip_object_value {
      JSON_LOG(std::format("skip_object_value."));
      insertKeyAndEmptyValue(key_value_map, key_value_linked, partial_pks, partial_pke, isInBuffer(partial_pks, state), extra_state);

      JSON_LOG("fgoto main");
      fgoto main;
    }

    action skip_boolean_value {
      JSON_LOG(std::format("skip_boolean_value."));
      insertKeyAndEmptyValue(key_value_map, key_value_linked, partial_pks, partial_pke, isInBuffer(partial_pks, state), extra_state);
    }

    action skip_null_value {
      JSON_LOG(std::format("skip_null_value."));
      insertKeyAndEmptyValue(key_value_map, key_value_linked, partial_pks, partial_pke, isInBuffer(partial_pks, state), extra_state);
    }

    #####################################################

    action skip_array_object_value {
      extra_state->square_bracket_count_ = 0;
      JSON_LOG(std::format("skip_array_object_value"));
      JSON_LOG("fgoto main");
      fgoto main;
    }

    action skip_array_boolean_value {
      JSON_LOG(std::format("skip_array_boolean_value: {}", std::string_view(ts, te - ts)));
    }

    action skip_array_null_value {
      JSON_LOG(std::format("skip_array_null_value: {}", std::string_view(ts, te - ts)));
    }

    WS = [ \t\r\n];
    
    main := |*
      WS => skip;
      '{' => skip;
      '}' => skip;
      ',' => skip;
      '[' => {
        JSON_LOG("start with array. fgoto value");
        p = ts;
        fhold;
        fgoto value;
      };
      ']' => skip;
      '"' => {
        JSON_LOG("fgoto key");
        fgoto key;
      };
      any => error;
    *|;

    key := |*
      # Try to buffer a complete key as much as possible, but also limit the length to prevent excessive memory usage.
      # A complete key is useful for finding key exactly.
      ([^"] | '\\"'){1,1024} => {
        if(partial_pks == nullptr) {
          partial_pks = ts;
        }
        partial_pke = te;
      };
      '"' => {
        JSON_LOG(std::format("fgoto split_colon. key: {}", std::string_view(partial_pks, partial_pke)));
        fgoto split_colon;
      };
    *|;

    split_colon := |*
      WS => skip;
      ':' => {
        JSON_LOG(std::format("fgoto value"));
        fgoto value;
      };
      any => error;
    *|;

    value := |*
      WS => skip;
      '[' => { 
        insertKeyAndEmptyValue(key_value_map, key_value_linked, partial_pks, partial_pke, isInBuffer(partial_pks, state), extra_state);
        JSON_LOG(std::format("fgoto array."));
        extra_state->square_bracket_count_++;
        fgoto array;
      };
      '}' | ',' => {
        JSON_LOG("end value, fgoto main");
        fgoto main;
      };
      '{' => skip_object_value;
      'true' => skip_boolean_value;
      'false' => skip_boolean_value;
      'null' => skip_null_value;
      '-'? [0-9] => {
        JSON_LOG(std::format("fgoto number_value: {}", std::string_view(ts, te - ts)));
        fgoto number_value;
      };
      '"' => {
        JSON_LOG(std::format("fgoto string_value"));
        fgoto string_value;
      };
      any => error;
    *|;

    number_value := |*
      [0-9] => {};
      '.' => {};
      any => {
        JSON_LOG(std::format("skip_number_value."));
        insertKeyAndEmptyValue(key_value_map, key_value_linked, partial_pks, partial_pke, isInBuffer(partial_pks, state), extra_state);
        fhold;
        fgoto value;
      };
    *|;

    string_value := |*
      [^"] | '\\"' => {
        if(partial_pvs == nullptr) {
          partial_pvs = ts;
        }
        partial_pve = te;
      };
      '"' => {
        std::string_view key_view(partial_pks, partial_pke);
        std::string_view value_view(partial_pvs, partial_pve);
        std::string buffer;

        // Insert the key-value pair
        if(extra_state->in_partial_) {
          // Decode the key
          jsDecodeStream(key_view, buffer, *extra_state->js_decode_key_state_, true);
          extra_state->escape_buffer_.emplace_front(std::move(buffer));
          key_view = extra_state->escape_buffer_.front();
          extra_state->js_decode_key_state_ = jsDecodeNewStream();

          // Decode the value
          jsDecodeStream(value_view, buffer, *extra_state->js_decode_value_state_, true);
          extra_state->escape_buffer_.emplace_front(std::move(buffer));
          value_view = extra_state->escape_buffer_.front();
          extra_state->js_decode_value_state_= jsDecodeNewStream();

          key_value_linked.emplace_back(extra_state->kv_pair_count_, true, key_view, value_view);
        } else {
          // Decode the key
          bool isKeyInBuffer = isInBuffer(partial_pks, state);
          if(isKeyInBuffer) {
            if(jsDecode(key_view, buffer)) {
              extra_state->escape_buffer_.emplace_front(std::move(buffer));
              key_view = extra_state->escape_buffer_.front();
            } else {
              extra_state->escape_buffer_.emplace_front(std::string(key_view));
              key_view = extra_state->escape_buffer_.front();
            } 
          } else {
            if(jsDecode(key_view, buffer)) {
              extra_state->escape_buffer_.emplace_front(std::move(buffer));
              key_view = extra_state->escape_buffer_.front();
            }
          }

          // Decode the value
          if(jsDecode(value_view, buffer)) {
            extra_state->escape_buffer_.emplace_front(std::move(buffer));
            value_view = extra_state->escape_buffer_.front();
          }

          key_value_map.insert({key_view, value_view});
          key_value_linked.emplace_back(extra_state->kv_pair_count_, false, key_view, value_view);
        }

        // Reset the partial pointers
        extra_state->in_partial_ = false;
        extra_state->kv_pair_count_++;
        partial_pks = nullptr;
        partial_pke = nullptr;
        partial_pvs = nullptr;
        partial_pve = nullptr;

        JSON_LOG(std::format("add_string_value. insert key-value: {}: {}", key_view, value_view));
        fgoto value;
      };
    *|;

    array := |*
      WS => skip;
      '[' => {
        JSON_LOG("array nesting open");
        extra_state->square_bracket_count_++;
      };
      ']' => {
        JSON_LOG("array nesting close");
        extra_state->square_bracket_count_--;
        if (extra_state->square_bracket_count_ == 0) {
          JSON_LOG("fgoto main");
          fgoto main;
        }
      };
      '{' => skip_array_object_value;
      '}' => skip;
      ',' => skip;
      'true' => skip_array_boolean_value;
      'false' => skip_array_boolean_value;
      'null' => skip_array_null_value;
      '-'? [0-9] => { fgoto array_number_value; };
      '"' => { fgoto array_string_value; };
      any => error;
    *|;

    array_number_value := |*
      [0-9] => {};
      '.' => {};
      any => {
        fhold;
        fgoto array;
      };
    *|;

    array_string_value := |*
      [^"] | '\\"' => {
        if(partial_pvs == nullptr) {
          partial_pvs = ts;
        }
        partial_pve = te;
      };
      '"' => {
        std::string_view value_view(partial_pvs, partial_pve);
        std::string buffer;

        // Insert the key-value pair
        if(extra_state->in_partial_) {
          // Decode the value
          jsDecodeStream(value_view, buffer, *extra_state->js_decode_value_state_, true);
          extra_state->escape_buffer_.emplace_front(std::move(buffer));
          value_view = extra_state->escape_buffer_.front();
          extra_state->js_decode_value_state_ = jsDecodeNewStream();

          key_value_linked.emplace_back(extra_state->kv_pair_count_, true, "", value_view);
        } else {
          // Decode the value
          if(jsDecode(value_view, buffer)) {
            extra_state->escape_buffer_.emplace_front(std::move(buffer));
            value_view = extra_state->escape_buffer_.front();
          }

          key_value_map.insert({"", value_view});
          key_value_linked.emplace_back(extra_state->kv_pair_count_, false, "", value_view);
        }

        // Reset the partial pointers
        extra_state->in_partial_ = false;
        extra_state->kv_pair_count_++;
        partial_pvs = nullptr;
        partial_pve = nullptr;

        JSON_LOG(std::format("add_string_value. insert key-value: {}: {}", "", value_view));
        fgoto array;
      };
    *|;
}%%

%% write data;

struct ParseJsonExtraState {
  int square_bracket_count_{0};
  uint32_t kv_pair_count_{0};
  bool in_partial_{false};
  std::forward_list<std::string> escape_buffer_;
  std::unique_ptr<Wge::Transformation::StreamState, std::function<void(Wge::Transformation::StreamState*)>> js_decode_key_state_;
  std::unique_ptr<Wge::Transformation::StreamState, std::function<void(Wge::Transformation::StreamState*)>> js_decode_value_state_;
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
parseJsonNewStream() {
  auto state = Wge::Transformation::newStreamWithExtraState<ParseJsonExtraState>();
  state->buffer_.reserve(1024);
  ParseJsonExtraState* extra_state =
      reinterpret_cast<ParseJsonExtraState*>(state->extra_state_buffer_.data());
  extra_state->js_decode_key_state_ = jsDecodeNewStream();
  extra_state->js_decode_value_state_ = jsDecodeNewStream();

  return state;
}

static bool isInBuffer(const char* p, Wge::Transformation::StreamState& state) {
  if(p >= state.buffer_.data() &&
     p < state.buffer_.data() + state.buffer_.size()) {
    return true;
  }
  return false;
}

static void insertKeyAndEmptyValue(std::unordered_multimap<std::string_view, std::string_view>& key_value_map,
                std::list<Wge::Common::Ragel::KeyValuePair>& key_value_linked, const char*& partial_pks,
                const char*& partial_pke, bool isKeyInBuffer, ParseJsonExtraState *extra_state) {
  std::string_view key_view(partial_pks, partial_pke);
  std::string buffer;

  // Insert the key-value pair
  if(extra_state->in_partial_) {
    // Decode the key
    jsDecodeStream(key_view, buffer, *extra_state->js_decode_key_state_, true);
    extra_state->escape_buffer_.emplace_front(std::move(buffer));
    key_view = extra_state->escape_buffer_.front();
    extra_state->js_decode_key_state_ = jsDecodeNewStream();

    key_value_linked.emplace_back(extra_state->kv_pair_count_, true, key_view, "");
    JSON_LOG(std::format("insertKeyAndEmptyValue, in_partial: true, key:{}", key_view));
  } else {
    // Decode the key
    if(isKeyInBuffer) {
      // The buffer of stream state is maintained automatically. That means the
      // buffer will be cleared as soon as it is no longer needed. So, if the 
      // key is in the buffer, we ensure it is stored in the escape buffer to 
      // avoid invalidating the original key_view.
      if(jsDecode(key_view, buffer)) {
        extra_state->escape_buffer_.emplace_front(std::move(buffer));
        key_view = extra_state->escape_buffer_.front();
      } else {
        extra_state->escape_buffer_.emplace_front(std::string(key_view));
        key_view = extra_state->escape_buffer_.front();
      }
    } else {
      if(jsDecode(key_view, buffer)) {
        extra_state->escape_buffer_.emplace_front(std::move(buffer));
        key_view = extra_state->escape_buffer_.front();
      }
    }

    key_value_map.insert({key_view, ""});
    key_value_linked.emplace_back(extra_state->kv_pair_count_, false, key_view, "");
    JSON_LOG(std::format("insertKeyAndEmptyValue, in_partial: false, key:{}", key_view));
  }

  // Reset the partial pointers
  extra_state->in_partial_ = false;
  extra_state->kv_pair_count_++;
  partial_pks = nullptr;
  partial_pke = nullptr;
}

static Wge::Transformation::StreamResult
parseJsonStream(std::string_view input,
                std::unordered_multimap<std::string_view, std::string_view>& key_value_map,
                std::list<Wge::Common::Ragel::KeyValuePair>& key_value_linked,
                Wge::Transformation::StreamState& state, bool end_stream) {
  using namespace Wge::Transformation;

  // The stream is not valid
  if (state.state_.test(static_cast<size_t>(StreamState::State::INVALID)))
    [[unlikely]] { return StreamResult::INVALID_INPUT; }

  // The stream is complete, no more data to process
  if (state.state_.test(static_cast<size_t>(StreamState::State::COMPLETE)))
    [[unlikely]] { return StreamResult::SUCCESS; }

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = end_stream ? pe : nullptr;
  const char *ts, *te;
  int cs, act;

  auto* extra_state = reinterpret_cast<ParseJsonExtraState*>(state.extra_state_buffer_.data());
  extra_state->escape_buffer_.clear();

  const char* partial_pks = nullptr; // Pointer to the start of the partial key
  const char* partial_pke = nullptr; // Pointer to the end of the partial key
  const char* partial_pvs = nullptr; // Pointer to the start of the partial value
  const char* partial_pve = nullptr; // Pointer to the end of the partial value
  bool error = false;

  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
      // clang-format on

      std::string_view partial_key_view;
      std::string_view partial_value_view;
      if (partial_pks && partial_pke) {
        partial_key_view = std::string_view(partial_pks, partial_pke);
        std::string buffer;

        // Decode the key
        jsDecodeStream(partial_key_view, buffer, *extra_state->js_decode_key_state_, false);
        extra_state->escape_buffer_.emplace_front(std::move(buffer));
        partial_key_view = extra_state->escape_buffer_.front();

        extra_state->in_partial_ = true;
      }

      if (partial_pvs && partial_pve) {
        partial_value_view = std::string_view(partial_pvs, partial_pve);
        std::string buffer;

        // Decode the value
        jsDecodeStream(partial_value_view, buffer, *extra_state->js_decode_value_state_, false);
        extra_state->escape_buffer_.emplace_front(std::move(buffer));
        partial_value_view = extra_state->escape_buffer_.front();

        extra_state->in_partial_ = true;
      }

      const bool has_new_partial = !partial_key_view.empty() || !partial_value_view.empty();
      if (has_new_partial) {
        key_value_linked.emplace_back(extra_state->kv_pair_count_, true, partial_key_view,
                                      partial_value_view);
      }

      return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
    }

#undef JSON_LOG