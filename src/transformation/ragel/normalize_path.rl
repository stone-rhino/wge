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

#include <assert.h>

#include "src/transformation/stream_util.h"

#define SLASH '/'
#define DOT '.'

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
    if(ts == ps) {
      result.resize(input.size());
      r = result.data();
      if(ts > input.data()){
        memcpy(r, input.data(), ts - input.data());
        r += ts - input.data();
      }
      p = ts;
      fhold;
      fgoto transformation;
    } else {
      // Hold the slash for the other cases
      fhold;
    }
  }

  action exec_transformation_if_end_with_dot {
    if(te == eof) {
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
  }

  action exec_transformation_if_start_with_dot_and_eof {
    if(ts == ps && te == eof) {
      result.resize(input.size());
      r = result.data();
      if(ts > input.data()){
        memcpy(r, input.data(), ts - input.data());
        r += ts - input.data();
      }
      p = ts;
      fhold;
      fgoto transformation;
    }  else {
      // Hold the slash for the other cases
      fhold;
    }
  }

  SLASH = '/';
  DOT = '.';

  # prescan
  main := |*
    SLASH DOT SLASH => exec_transformation;
    SLASH DOT DOT SLASH => exec_transformation;
    SLASH DOT DOT => exec_transformation_if_end_with_dot;
    SLASH DOT => exec_transformation_if_end_with_dot;
    SLASH SLASH+ => exec_transformation;
    DOT SLASH => exec_transformation_if_start_with_dot;
    DOT DOT SLASH => exec_transformation_if_start_with_dot_and_eof;
    any => skip;
  *|;

  transformation := |*
    SLASH+ DOT SLASH+ => {
      if(ts == ps) {
        *r++ = SLASH;
      }

      // Hold the SLASH for the other cases
      fhold;

      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT SLASH+:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ DOT DOT SLASH+ => {
      if(ts == ps) {
        *r++ = SLASH;
      }

      // Remove the last directory
      removeLastDir(r, result);

      if(te != eof) {
        // Hold the SLASH for the other cases
        fhold;
      }

      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT DOT SLASH+:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ DOT DOT => {
      if(ts == ps) {
        *r++ = SLASH;
      }

      if(te != eof) {
        if(ts != ps) {
          *r++ = SLASH;
        }
        *r++ = DOT;
        *r++ = DOT;
      } else {
        // Remove the last directory
        removeLastDir(r, result);
      }

      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT DOT:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ DOT  => {
      *r++ = SLASH;
      if(te != eof) {
        *r++ = DOT;
      }

      NORMALIZE_PATH_LOG(std::format("SLASH+ DOT:{}",std::string_view(result.data(), r - result.data())));
    };
    DOT SLASH+ => {
      if(ts != ps) {
        *r++ = DOT;
      }

      fhold;

      NORMALIZE_PATH_LOG(std::format("DOT SLASH+:{}",std::string_view(result.data(), r - result.data())));
    };
    DOT DOT SLASH+ => {
      *r++ = DOT;
      *r++ = DOT;

      fhold;

      NORMALIZE_PATH_LOG(std::format("DOT DOT SLASH+:{}",std::string_view(result.data(), r - result.data())));
    };
    SLASH+ => {
      if(ts == ps) {
        *r++ = SLASH;
      } else {
        if(r > result.data() && *(r-1) != SLASH) {
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
}%%

%% write data;
// clang-format on

static void removeLastDir(char*& r, const std::string& result) {
  std::string_view result_view(result.data(), r - result.data());
  auto pos = result_view.find_last_of(SLASH);
  if (pos == result_view.size() - 1) {
    // If the last character is a slash, we need to skip it
    pos = result_view.find_last_of(SLASH, pos - 1);
  }
  if (pos != std::string_view::npos) {
    if (pos == result_view.size() - 3 && result_view[pos + 1] == '.' &&
        result_view[pos + 2] == '.') {
      // If the last part is "/..", we need to append "/.."
      *r++ = SLASH;
      *r++ = DOT;
      *r++ = DOT;
    } else {
      r = const_cast<char*>(result_view.data() + pos + 1);
    }
  } else {
    if (result_view.empty()) {
      *r++ = DOT;
      *r++ = DOT;
    } else {
      if (result_view == "..") {
        // If the result is just "..", we need to append "/.."
        *r++ = SLASH;
        *r++ = DOT;
        *r++ = DOT;
      } else {
        r = const_cast<char*>(result_view.data());
      }
    }
  }
}

static void removeLastDir(std::string& result) {
  // Reserve space for "/.."
  size_t len = result.size();
  result.resize(len + 3);

  char* r = result.data() + len;
  removeLastDir(r, result);

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
    // If the result is empty, we set the result to '.'
    if (r == result.data()) {
      *r++ = DOT;
    }

    result.resize(r - result.data());

    // Remove the last directory if the last part is "/../", "../" or "./"
    if (result.ends_with("/../") || result == "../" || result == "./") {
      result.pop_back();
    }
    return true;
  }

  return false;
}

static std::unique_ptr<Wge::Transformation::StreamState,
                       std::function<void(Wge::Transformation::StreamState*)>>
normalizePathNewStream() {
  return std::make_unique<Wge::Transformation::StreamState>(4096);
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

  // It's difficult to normalize the path as stream parse mode.
  // Example: "/a/b/c/../d/./e"
  // When we parse "/../", we need to know the previous part "/a/b/c/" to remove the last directory.
  // So we need to hold the previous part until we reach the end of the stream or the next
  // characters sequence that is "/../". If the previous part is so long, the memory usage will be
  // high.
  // So we will not support the stream parse mode for now. But we will buffer the input and process
  // it when the stream is complete.
  // To avoid the memory usage, Follow the limit of linux path length, we will reserve a buffer size
  // of 4096 bytes.
  if (state.buffer_.size() + input.size() > 4096) {
    state.state_.set(static_cast<size_t>(StreamState::State::INVALID));
    return StreamResult::INVALID_INPUT;
  }

  state.buffer_.append(input);

  if (end_stream) {
    auto ret = normalizePath(state.buffer_, result);
    if (!ret) {
      result = std::move(state.buffer_);
    }
    state.buffer_.clear();
    state.state_.set(static_cast<size_t>(Wge::Transformation::StreamState::State::COMPLETE));
    return StreamResult::SUCCESS;
  } else {
    return StreamResult::NEED_MORE_DATA;
  }
}

#undef SLASH
#undef DOT
#undef NORMALIZE_PATH_LOG