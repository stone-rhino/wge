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
   * Load patterns from the pattern
   * @param pattern the pattern
   * @param literal whether the pattern is literal
   * @param som_leftmost whether enable HS_FLAG_SOM_LEFTMOST flag when compile
   */
  HsDataBase(const std::string& pattern, bool literal, bool som_leftmost);

  /**
   * Load patterns from the specified file.
   * Each line in the file is a pattern.
   * @param ifs the file stream
   * @param literal whether the patterns are literal
   * @param som_leftmost whether enable HS_FLAG_SOM_LEFTMOST flag when compile
   */
  HsDataBase(std::ifstream& ifs, bool literal, bool som_leftmost);

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
  void compile();
  static Scratch main_scratch_;

private:
  Database db_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity