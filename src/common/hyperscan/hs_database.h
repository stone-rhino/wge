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

#include <array>
#include <fstream>
#include <string>

#include <hs/hs.h>

#include "expression.h"
#include "scratch.h"

#include "../assert.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
/**
 * Hyperscan database
 */
class HsDataBase {
public:
  /**
   * Load a pattern from a string.
   * @param pattern the pattern
   * @param literal whether the pattern is literal
   * @param som_leftmost whether enable HS_FLAG_SOM_LEFTMOST flag when compile
   * @param support_stream whether support stream mode
   */
  HsDataBase(const std::string& pattern, bool literal, bool som_leftmost,
             bool support_stream = false);

  /**
   * Load patterns form a vector of string_view.
   * @param patterns the patterns
   * @param literal whether the patterns are literal
   * @param som_leftmost whether enable HS_FLAG_SOM_LEFTMOST flag when compile
   * @param support_stream whether support stream mode
   */
  HsDataBase(const std::vector<std::string_view>& patterns, bool literal, bool som_leftmost,
             bool support_stream = false);

  /**
   * Load patterns from the specified file.
   * Each line in the file is a pattern.
   * @param ifs the file stream
   * @param literal whether the patterns are literal
   * @param som_leftmost whether enable HS_FLAG_SOM_LEFTMOST flag when compile
   * @param support_stream whether support stream mode
   */
  HsDataBase(std::ifstream& ifs, bool literal, bool som_leftmost, bool support_stream = false);

public:
  const hs_database_t* blockNative() const { return db_.block_db_; }
  const hs_database_t* streamNative() const { return db_.stream_db_; }

  uint64_t getRealId(unsigned int id) const { return db_.expressions_.getRealId(id); }

  const Pcre::PatternList& getPcrePatternList() const {
    return db_.expressions_.getPcrePatternList();
  }

  static Scratch& mainScratch() { return main_scratch_; }

private:
  struct Database {
    hs_database_t* block_db_{nullptr};
    hs_database_t* stream_db_{nullptr};
    ExpressionList expressions_;
    Database(bool literal) : expressions_(literal) {}
    ~Database() {
      if (block_db_) {
        ::hs_free_database(block_db_);
      }
      if (stream_db_) {
        ::hs_free_database(stream_db_);
      }
    }
  };

private:
  void compile(bool support_stream);
  static Scratch main_scratch_;

private:
  Database db_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity