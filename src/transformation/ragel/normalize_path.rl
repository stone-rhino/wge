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

#include <iostream>
#include <string>
#include <string_view>

#include "src/transformation/stream_util.h"

#define SLASH '/'

#ifndef ENABLE_NORMALIZE_PATH_DEBUG_LOG
#define ENABLE_NORMALIZE_PATH_DEBUG_LOG 0
#endif

#if ENABLE_NORMALIZE_PATH_DEBUG_LOG
#include <format>
#include <iostream>
#define NORMALIZE_PATH_LOG(x) std::cout << x << std::endl;
#else
#define NORMALIZE_PATH_LOG(x)
#endif

// Removes multiple slashes, directory self-references, and directory back-references (except when
// at the beginning of the input) from input string.
// clang-format off
%%{
  machine normalize_path;

  action skip {}

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

  action exec_transformation_if_start_with_dot {
    if(ts == ps ) {
      result.resize(input.size());
      r = result.data();
      if(ts > input.data()){
        memcpy(r, input.data(), ts - input.data());
        r += ts - input.data();
      }
      p = ts;
      fhold;
      fgoto transformation_if_start_with_dot;
    }
  }

  SLASH = '/';
  DOT = '.';

  # prescan
  main := |*
    SLASH DOT+ [^./] => skip; 
    SLASH DOT DOT => exec_transformation;
    SLASH DOT => exec_transformation;
    SLASH SLASH => exec_transformation;
    DOT [^./] => skip;
    DOT => exec_transformation_if_start_with_dot;
    any => skip;
  *|;

  transformation := |*

    SLASH+ DOT+ [^./] => {
      *r++ = SLASH;
      *r++ = '.';
      fhold;
    };
    SLASH+ DOT DOT {
      if (ts == ps) {
        *r++ = SLASH;
      } else {
        removeLastDir(r, result.data()); 
      }

      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT DOT:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ DOT => { 
      if(ts == ps) { 
        *r++ = SLASH;
      }
      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ => {
      // Ensure that after removing the last directory, if the input is not start 
      // with a slash then the result is not start with a slash too
      if(ts == ps) {
        *r++ = SLASH; 
      } else if (r > result.data()) {
        if(*(r - 1) != SLASH) {
          *r++ = SLASH; 
        }
      }

      NORMALIZE_PATH_LOG(std::format("SLASH+:{}",std::string_view(result.data(), r - result.data())));
    };
    any => { 
      *r++ = fc;
      NORMALIZE_PATH_LOG(std::format("any:{}",std::string_view(result.data(), r - result.data())));
    };
  *|;

  transformation_if_start_with_dot := |*
    DOT SLASH+ => {
      fgoto transformation;
    };
    DOT DOT => {
      *r++ = '.';
      *r++ = '.';
      fgoto transformation;
    };
  *|;
}%%

%% write data;
// clang-format on

static void removeLastDir(char*& input, const char* start_input) {
  if (input > start_input) {
    char* p = input - 1;
    while (p > start_input && *p != SLASH) {
      p--;
    }

    if (p + 2 == input && *p == '.' && *(p + 1) == '.') {
      *input++ = SLASH;
      *input++ = '.';
      *input++ = '.';
    } else {
      input = p;
      if (input == start_input && *input == SLASH) {
        input++;
      }
    }
  } else {
    if (*start_input == SLASH) {
      *input++ = SLASH;
    }
    *input++ = '.';
    *input++ = '.';
  }
}

static void removeLastDir(std::string& result) {
  // Reserve space for "/.."
  size_t len = result.size();
  result.resize(len + 3);

  char* r = result.data() + len;
  removeLastDir(r, result.data());

  result.resize(r - result.data());
}

static bool normalizePath(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* ps = p;
  const char* pe = p + input.size();
  const char* eof = pe;
  const char *ts, *te;
  int cs, act;

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
  machine normalize_path_stream;

  SLASH = '/';
  DOT = '.';

  main := |*
    SLASH+ DOT+ [^./] => {
      result += "/.";
      fhold;
    };
    SLASH+ DOT DOT {
      if (result.empty() && is_invoked_remove_last_dir == false) {
        result += SLASH;
      } else {
        removeLastDir(result);
        is_invoked_remove_last_dir = true;
      }
    };
    SLASH+ DOT => {
      if(result.empty() && is_invoked_remove_last_dir == false) {
        result += SLASH;
      }
    };
    SLASH+ => {
      // Ensure that after removing the last directory, if the input is not start
      // with a slash then the result is not start with a slash too
      if(result.empty() && is_invoked_remove_last_dir == false) {
        result += SLASH;
      } else {
        if(!result.empty() && result.back() != SLASH) {
          result += SLASH;
        }
      }
    };
    DOT SLASH+ => {
      if(!result.empty()) {
        result += "./";
      }
    };
    DOT DOT => {
      result += "..";
    };
    DOT [^.] => {
      result += ".";
      result += fc;
    };
    DOT => {};
    any => {
      result += fc;
    };
  *|;
}%%

%% write data;
// clang-format on

struct NormalizePathExtraState {
  bool is_invoked_remove_last_dir_{false};
};

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
normalizePathNewStream() {
  return Wge::Transformation::newStreamWithExtraState<NormalizePathExtraState>();
}

static Wge::Transformation::StreamResult
normalizePathStream(std::string_view input, std::string& result,
                    Wge::Transformation::StreamState& state, bool end_stream) {
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

  auto* extra_state = reinterpret_cast<NormalizePathExtraState*>(state.extra_state_buffer_.data());
  bool& is_invoked_remove_last_dir = extra_state->is_invoked_remove_last_dir_;

  // clang-format off
  %% write init;
  recoverStreamState(state, input, ps, pe, eof, p, cs, act, ts, te, end_stream);
  %% write exec;
  // clang-format on

  return saveStreamState(state, cs, act, ps, pe, ts, te, end_stream);
}

#undef SLASH
#undef NORMALIZE_PATH_LOG