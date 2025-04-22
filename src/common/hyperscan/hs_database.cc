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
#include "hs_database.h"

#include <future>

#include "../assert.h"
#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
Scratch HsDataBase::main_scratch_;

HsDataBase::HsDataBase(const std::string& pattern, bool literal, bool som_leftmost,
                       bool support_stream)
    : db_(literal) {
  unsigned int flag = HS_FLAG_CASELESS;
  if (som_leftmost) {
    flag |= HS_FLAG_SOM_LEFTMOST;
  }
  if (!db_.expressions_.literal()) {
    flag |= HS_FLAG_UTF8;
  }
  db_.expressions_.add({pattern, flag, 0}, true);
  compile(support_stream);
}

HsDataBase::HsDataBase(const std::vector<std::string_view>& patterns, bool literal,
                       bool som_leftmost, bool support_stream)
    : db_(literal) {
  unsigned int flag = HS_FLAG_CASELESS;
  if (som_leftmost) {
    flag |= HS_FLAG_SOM_LEFTMOST;
  }
  if (!literal) {
    flag |= HS_FLAG_UTF8;
  }
  size_t i = 0;
  for (; i < patterns.size() - 1; ++i) {
    db_.expressions_.add({std::string(patterns[i]), flag, i}, false);
  }
  db_.expressions_.add({std::string(patterns[patterns.size() - 1]), flag, i}, true);

  compile(support_stream);
}

HsDataBase::HsDataBase(std::ifstream& ifs, bool literal, bool som_leftmost, bool support_stream)
    : db_(literal) {
  if (db_.expressions_.load(ifs, true, som_leftmost, false)) {
    compile(support_stream);
  }
}

void HsDataBase::compile(bool support_stream) {
  assert(db_.expressions_.size());

  if (support_stream) {
    // compile block mode
    auto block_compiler = std::async([&]() {
      hs_compile_error_t* compile_err;
      hs_error_t err;
      if (db_.expressions_.literal()) {
        err = ::hs_compile_lit_multi(db_.expressions_.exprRawData(),
                                     db_.expressions_.flagsRawData(), db_.expressions_.idsRawData(),
                                     db_.expressions_.exprLenRawData(), db_.expressions_.size(),
                                     HS_MODE_BLOCK, nullptr, &db_.block_db_, &compile_err);

      } else {
        err = ::hs_compile_multi(db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
                                 db_.expressions_.idsRawData(), db_.expressions_.size(),
                                 HS_MODE_BLOCK, nullptr, &db_.block_db_, &compile_err);
      }

      if (err == HS_COMPILER_ERROR) {
        SRSECURITY_LOG_ERROR("compile error: {} index: {} id: {} expression: {}",
                             compile_err->message, compile_err->expression,
                             db_.expressions_.getRealId(compile_err->expression),
                             db_.expressions_.exprRawData()[compile_err->expression]);
        ::hs_free_compile_error(compile_err);
      }
    });

    // compile stream mode
    auto stream_compiler = std::async([&]() {
      hs_compile_error_t* compile_err;
      hs_error_t err;
      if (db_.expressions_.literal()) {
        err = ::hs_compile_lit_multi(db_.expressions_.exprRawData(),
                                     db_.expressions_.flagsRawData(), db_.expressions_.idsRawData(),
                                     db_.expressions_.exprLenRawData(), db_.expressions_.size(),
                                     HS_MODE_STREAM | HS_MODE_SOM_HORIZON_SMALL, nullptr,
                                     &db_.stream_db_, &compile_err);
      } else {
        err = ::hs_compile_multi(db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
                                 db_.expressions_.idsRawData(), db_.expressions_.size(),
                                 HS_MODE_STREAM | HS_MODE_SOM_HORIZON_SMALL, nullptr,
                                 &db_.stream_db_, &compile_err);
      }

      if (err == HS_COMPILER_ERROR) {
        SRSECURITY_LOG_ERROR("compile error: {} index: {} id: {} expression: {}",
                             compile_err->message, compile_err->expression,
                             db_.expressions_.getRealId(compile_err->expression),
                             db_.expressions_.exprRawData()[compile_err->expression]);
        ::hs_free_compile_error(compile_err);
      }
    });

    block_compiler.wait();
    stream_compiler.wait();

    // realloc the main scratch space
    if (db_.block_db_ && db_.stream_db_) {
      main_scratch_.addBlock(db_.block_db_);
      main_scratch_.addStream(db_.stream_db_);
    }
  } else {
    hs_compile_error_t* compile_err;
    hs_error_t err;
    if (db_.expressions_.literal()) {
      err = ::hs_compile_lit_multi(db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
                                   db_.expressions_.idsRawData(), db_.expressions_.exprLenRawData(),
                                   db_.expressions_.size(), HS_MODE_BLOCK, nullptr, &db_.block_db_,
                                   &compile_err);

    } else {
      err = ::hs_compile_multi(db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
                               db_.expressions_.idsRawData(), db_.expressions_.size(),
                               HS_MODE_BLOCK, nullptr, &db_.block_db_, &compile_err);
    }

    if (err == HS_COMPILER_ERROR) {
      SRSECURITY_LOG_ERROR("compile error: {} index: {} id: {} expression: {}",
                           compile_err->message, compile_err->expression,
                           db_.expressions_.getRealId(compile_err->expression),
                           db_.expressions_.exprRawData()[compile_err->expression]);
      ::hs_free_compile_error(compile_err);
    }

    // realloc the main scratch space
    if (db_.block_db_) {
      main_scratch_.addBlock(db_.block_db_);
    }
  }
}
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity