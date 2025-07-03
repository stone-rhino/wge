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

#include <filesystem>
#include <fstream>
#include <future>

#include "../assert.h"
#include "../log.h"

namespace Wge {
namespace Common {
namespace Hyperscan {
Scratch HsDataBase::main_scratch_;

HsDataBase::HsDataBase(const std::string& pattern, bool literal, bool case_less, bool som_leftmost,
                       bool prefilter, bool support_stream, const char* serialize_dir)
    : db_(literal) {
  unsigned int flag = HS_FLAG_SINGLEMATCH;
  if (case_less) {
    flag |= HS_FLAG_CASELESS;
  }
  if (som_leftmost) {
    flag |= HS_FLAG_SOM_LEFTMOST;
  }
  if (!db_.expressions_.literal()) {
    flag |= HS_FLAG_DOTALL;
    flag |= HS_FLAG_MULTILINE;
  }

  db_.expressions_.add({pattern, flag, 0}, prefilter, true);

  loadOrCompile(serialize_dir, support_stream);
}

HsDataBase::HsDataBase(const std::vector<std::string_view>& patterns, bool literal, bool case_less,
                       bool som_leftmost, bool prefilter, bool support_stream,
                       const char* serialize_dir)
    : db_(literal) {
  if (!patterns.empty()) {
    unsigned int flag = HS_FLAG_SINGLEMATCH;
    if (case_less) {
      flag |= HS_FLAG_CASELESS;
    }
    if (som_leftmost) {
      flag |= HS_FLAG_SOM_LEFTMOST;
    }
    if (!literal) {
      flag |= HS_FLAG_DOTALL;
      flag |= HS_FLAG_MULTILINE;
    }
    size_t i = 0;
    for (; i < patterns.size() - 1; ++i) {
      db_.expressions_.add({std::string(patterns[i]), flag, i}, prefilter, false);
    }
    db_.expressions_.add({std::string(patterns[patterns.size() - 1]), flag, i}, prefilter, true);

    loadOrCompile(serialize_dir, support_stream);
  }
}

HsDataBase::HsDataBase(const std::vector<std::string_view>& patterns,
                       const std::vector<uint64_t>& ids, bool literal, bool case_less,
                       bool som_leftmost, bool prefilter, bool support_stream,
                       const char* serialize_dir)
    : db_(literal) {
  if (!patterns.empty()) {
    assert(patterns.size() == ids.size());

    unsigned int flag = HS_FLAG_SINGLEMATCH;
    if (case_less) {
      flag |= HS_FLAG_CASELESS;
    }
    if (som_leftmost) {
      flag |= HS_FLAG_SOM_LEFTMOST;
    }
    if (!literal) {
      flag |= HS_FLAG_DOTALL;
      flag |= HS_FLAG_MULTILINE;
    }
    size_t i = 0;
    for (; i < patterns.size() - 1; ++i) {
      db_.expressions_.add({std::string(patterns[i]), flag, ids[i]}, prefilter, false);
    }
    db_.expressions_.add({std::string(patterns[patterns.size() - 1]), flag, ids[i]}, prefilter,
                         true);

    loadOrCompile(serialize_dir, support_stream);
  }
}

HsDataBase::HsDataBase(std::ifstream& ifs, bool literal, bool case_less, bool som_leftmost,
                       bool prefilter, bool support_stream, const char* serialize_dir)
    : db_(literal) {
  if (db_.expressions_.load(ifs, true, case_less, som_leftmost, prefilter, false)) {
    loadOrCompile(serialize_dir, support_stream);
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
        WGE_LOG_ERROR("compile error: {} index: {} id: {} expression: {}", compile_err->message,
                      compile_err->expression, db_.expressions_.getRealId(compile_err->expression),
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
        WGE_LOG_ERROR("compile error: {} index: {} id: {} expression: {}", compile_err->message,
                      compile_err->expression, db_.expressions_.getRealId(compile_err->expression),
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
      WGE_LOG_ERROR("compile error: {} index: {} id: {} expression: {}", compile_err->message,
                    compile_err->expression, db_.expressions_.getRealId(compile_err->expression),
                    db_.expressions_.exprRawData()[compile_err->expression]);
      ::hs_free_compile_error(compile_err);
    }

    // realloc the main scratch space
    if (db_.block_db_) {
      main_scratch_.addBlock(db_.block_db_);
    }
  }
}

bool HsDataBase::loadFromSerialize(const char* serialize_dir, bool support_stream) {
  auto load = [](const std::string& file, hs_database_t** db) -> bool {
    // Read the serialize file
    WGE_LOG_INFO("Loading hyperscan database from {}", file);
    std::ifstream ifs(file, std::ios::binary);
    if (!ifs.is_open()) {
      WGE_LOG_ERROR("Failed to open hyperscan database file: {}", file);
      return false;
    }
    std::string file_content((std::istreambuf_iterator<char>(ifs)),
                             std::istreambuf_iterator<char>());

    // Load db
    hs_error_t err = ::hs_deserialize_database(file_content.data(), file_content.size(), db);
    if (err != HS_SUCCESS) {
      WGE_LOG_ERROR("Failed to load hyperscan database from {}", file);
      return false;
    }

    return true;
  };

  std::string serialize_block_file = makeBlockSerializeFilePath(serialize_dir);
  if (load(serialize_block_file, &db_.block_db_)) {
    main_scratch_.addBlock(db_.block_db_);
  } else {
    return false;
  }

  if (support_stream) {
    std::string serialize_steam_file = makeStreamSerializeFilePath(serialize_dir);
    if (load(serialize_steam_file, &db_.stream_db_)) {
      main_scratch_.addStream(db_.stream_db_);
    } else {
      return false;
    }
  }

  return true;
}

void HsDataBase::serialize(const char* serialize_dir, bool support_stream) const {
  auto save = [](const std::string& file, hs_database_t* db) {
    std::ofstream ofs(file, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) {
      WGE_LOG_ERROR("Failed to open hyperscan database file for writing: {}", file);
      return;
    }

    // Serialize block database to the file
    char* db_data = nullptr;
    size_t db_data_size = 0;
    hs_error_t err = ::hs_serialize_database(db, &db_data, &db_data_size);
    if (err != HS_SUCCESS) {
      WGE_LOG_ERROR("Failed to serialize hyperscan database to {}", file);
      return;
    }
    ofs.write(db_data, db_data_size);
    ::free(db_data);

    WGE_LOG_INFO("Serialized hyperscan database to {}", file);
  };

  // Create the directory
  if (!std::filesystem::exists(serialize_dir)) {
    std::filesystem::create_directories(serialize_dir);
  }

  std::string serialize_block_file = makeBlockSerializeFilePath(serialize_dir);
  save(serialize_block_file, db_.block_db_);
  if (support_stream) {
    std::string serialize_steam_file = makeStreamSerializeFilePath(serialize_dir);
    save(serialize_steam_file, db_.stream_db_);
  }
}

std::string HsDataBase::makeBlockSerializeFilePath(const char* serialize_dir) const {
  return std::string(serialize_dir) + "/" + expressions_sha1_ + ".bdb";
}

std::string HsDataBase::makeStreamSerializeFilePath(const char* serialize_dir) const {
  return std::string(serialize_dir) + "/" + expressions_sha1_ + ".sdb";
}

void HsDataBase::loadOrCompile(const char* serialize_dir, bool support_stream) {
  bool load_from_serialize = false;
  if (serialize_dir) {
    expressions_sha1_ = db_.expressions_.sha1();
    load_from_serialize = loadFromSerialize(serialize_dir, support_stream);
  }

  if (!load_from_serialize) {
    compile(support_stream);

    // Serialize the database if serialize_dir is specified
    if (serialize_dir) {
      serialize(serialize_dir, support_stream);
    }
  }
}
} // namespace Hyperscan
} // namespace Common
} // namespace Wge