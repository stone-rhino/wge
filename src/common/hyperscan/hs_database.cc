#include "hs_database.h"

#include <future>

#include "../assert.h"
#include "../log.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
Scratch HsDataBase::main_scratch_;

HsDataBase::HsDataBase(const std::string& pattern, bool literal, bool som_leftmost) : db_(literal) {
  unsigned int flag = HS_FLAG_CASELESS;
  if (som_leftmost) {
    flag |= HS_FLAG_SOM_LEFTMOST;
  }
  if (!db_.expressions_.literal()) {
    flag |= HS_FLAG_UTF8;
  }
  db_.expressions_.add({pattern, flag, 0});
  compile();
}

HsDataBase::HsDataBase(std::ifstream& ifs, bool literal, bool som_leftmost) : db_(literal) {
  if (db_.expressions_.load(ifs, true, som_leftmost, false)) {
    compile();
  }
}

void HsDataBase::compile() {
  ASSERT_IS_MAIN_THREAD();

  // compile block mode
  auto block_compiler = std::async([&]() {
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
  });

  // compile stream mode
  auto stream_compiler = std::async([&]() {
    hs_compile_error_t* compile_err;
    hs_error_t err;
    if (db_.expressions_.literal()) {
      err = ::hs_compile_lit_multi(
          db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
          db_.expressions_.idsRawData(), db_.expressions_.exprLenRawData(), db_.expressions_.size(),
          HS_MODE_STREAM | HS_MODE_SOM_HORIZON_SMALL, nullptr, &db_.stream_db_, &compile_err);
    } else {
      err = ::hs_compile_multi(db_.expressions_.exprRawData(), db_.expressions_.flagsRawData(),
                               db_.expressions_.idsRawData(), db_.expressions_.size(),
                               HS_MODE_STREAM | HS_MODE_SOM_HORIZON_SMALL, nullptr, &db_.stream_db_,
                               &compile_err);
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
    main_scratch_.add(db_.block_db_, db_.stream_db_);
  }
}
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity