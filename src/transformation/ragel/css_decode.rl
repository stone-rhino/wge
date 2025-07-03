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

#include "hex_decode.h"
#include "src/transformation/stream_util.h"

// clang-format off
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
// clang-format on

static void emitNumericEntity(char** r, const std::string& entity_value) {
  std::string decoded;
  std::string_view data(entity_value);

  // If length of entity_value is greater than 2, then use the last two from the end
  if (entity_value.size() > 2) {
    data = {entity_value.data() + entity_value.size() - 2, 2};
  }

  // If the length of entity_value is greater than 3, and the value of entity_value
  // is like "FFxx","0FFxx","00FFxx", we need do full width conversion
  bool full_width = false;
  if (entity_value.size() > 3) {
    switch (entity_value.size()) {
    case 4:
      full_width = (entity_value[0] == 'f' || entity_value[0] == 'F') &&
                   (entity_value[1] == 'f' || entity_value[1] == 'F');
      break;
    case 5:
      full_width = entity_value[0] == '0' && (entity_value[1] == 'f' || entity_value[1] == 'F') &&
                   (entity_value[2] == 'f' || entity_value[2] == 'F');
      break;
    case 6:
      full_width = entity_value[0] == '0' && entity_value[1] == '0' &&
                   (entity_value[1] == 'f' || entity_value[1] == 'F') &&
                   (entity_value[2] == 'f' || entity_value[2] == 'F');
      break;
    default:
      assert(false);
      break;
    }
  }

  hexDecode(data, decoded);
  if (full_width) {
    decoded.front() += 0x20;
  }
  memcpy(*r, decoded.data(), decoded.size());
  *r += decoded.size();
}

static bool cssDecode(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

  std::string entity_value;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  if (r) {
    result.resize(r - result.data());
    return true;
  }

  return false;
}

// clang-format off
%%{
  machine css_decode_stream;
  
  hex = [0-9a-fA-F];
  not_hex = [^0-9a-fA-F\n];

  main := |*
    '\\' hex{1,6} ' ' any => { entity_value = std::string(ts + 1, te - ts - 3); emitNumericEntity(result, entity_value); result += fc; };
    '\\' hex{1,6} => { entity_value = std::string(ts + 1, te - ts - 1); emitNumericEntity(result, entity_value); };
    '\\' not_hex => { result += fc; };
    '\\' '\n' => {}; # A newline character following backslash is ignored
    '\\' => {}; # The backslash is at the end of the input
    any => { result += fc; };
  *|;
}%%

%% write data;
// clang-format on

static void emitNumericEntity(std::string& result, const std::string& entity_value) {
  std::string decoded;
  std::string_view data(entity_value);

  // If length of entity_value is greater than 2, then use the last two from the end
  if (entity_value.size() > 2) {
    data = {entity_value.data() + entity_value.size() - 2, 2};
  }

  // If the length of entity_value is greater than 3, and the value of entity_value
  // is like "FFxx","0FFxx","00FFxx", we need do full width conversion
  bool full_width = false;
  if (entity_value.size() > 3) {
    switch (entity_value.size()) {
    case 4:
      full_width = (entity_value[0] == 'f' || entity_value[0] == 'F') &&
                   (entity_value[1] == 'f' || entity_value[1] == 'F');
      break;
    case 5:
      full_width = entity_value[0] == '0' && (entity_value[1] == 'f' || entity_value[1] == 'F') &&
                   (entity_value[2] == 'f' || entity_value[2] == 'F');
      break;
    case 6:
      full_width = entity_value[0] == '0' && entity_value[1] == '0' &&
                   (entity_value[1] == 'f' || entity_value[1] == 'F') &&
                   (entity_value[2] == 'f' || entity_value[2] == 'F');
      break;
    default:
      assert(false);
      break;
    }
  }

  hexDecode(data, decoded);
  if (full_width) {
    decoded.front() += 0x20;
  }
  result.append(decoded);
}

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
cssDecodeNewStream() {
  return std::make_unique<Wge::Transformation::StreamState>();
}

static Wge::Transformation::StreamResult cssDecodeStream(std::string_view input,
                                                         std::string& result,
                                                         Wge::Transformation::StreamState& state,
                                                         bool end_stream) {
  using namespace Wge::Transformation;

  // The stream is not valid
  if (state.state_.test(static_cast<size_t>(StreamState::State::INVALID)))
    [[unlikely]] { return StreamResult::INVALID_INPUT; }

  // The stream is complete, no more data to process
  if (state.state_.test(static_cast<size_t>(StreamState::State::COMPLETE)))
    [[unlikely]] { return StreamResult::SUCCESS; }

  // In the stream mode, we can't operate the raw pointer of the result directly simular to the
  // block mode since we can't guarantee reserve enough space in the result string. Instead, we
  // will use the string's append method to add the transformed data. Although this is less
  // efficient than using a raw pointer, it is necessary to ensure the safety of the stream
  // processing.
  result.reserve(result.size() + input.size());

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = end_stream ? pe : nullptr;
  const char *ts, *te;
  int cs, act;

  std::string entity_value;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}