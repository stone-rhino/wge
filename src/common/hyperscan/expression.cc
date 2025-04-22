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
#include "expression.h"

#include <fstream>
#include <regex>

#include <hs/hs.h>

#include "../log.h"

namespace Wge {
namespace Common {
namespace Hyperscan {
bool ExpressionList::load(std::ifstream& ifs, bool utf8, bool som_leftmost, bool multi_line) {
  assert(ifs.is_open());
  if (!ifs.is_open()) {
    return false;
  }

  constexpr size_t max_character = 2048;
  unsigned int flag = 0;
  flag |= HS_FLAG_CASELESS;

  if (utf8 && !literal_) {
    flag |= HS_FLAG_UTF8;
  }

  if (som_leftmost) {
    flag |= HS_FLAG_SOM_LEFTMOST;
  }

  if (multi_line && !literal_) {
    flag |= HS_FLAG_MULTILINE;
  }

  clear();

  std::string buffer;
  uint64_t id = 0;
  while (!ifs.eof() && ifs.good()) {
    buffer.resize(max_character);
    ifs.getline(buffer.data(), max_character);
    buffer.resize(ifs.gcount() == 0 ? 0 : ifs.gcount() - 1);

    if (buffer.empty()) {
      continue;
    }

    if (buffer == "##") {
      break;
    }

    if (buffer.starts_with("#")) {
      continue;
    }

    add({std::move(buffer), flag, id}, false);
    ++id;
  }

  // Must call initRawData() for the list to get raw data. It same as at last call add() with true.
  initRawData();

  return true;
}

void ExpressionList::add(Expression&& exp, bool init_raw_data) {
  auto iter = logic_id_map_.find(exp.id_);
  if (iter == logic_id_map_.end()) {
    // The hyperscan not supported complete pcre syntax, that means it can't process some
    // expression such as lookaround ahead/behind and backreference.
    // To support these pcre syntax, we use HS_FLAG_PREFILTER to complie first, hyperscan
    // transform the expressions so it can be processed. E.g the pattern (?<=hello)world may be
    // transform to \w+world. After the hyperscan matched the pattern, we use pcre to scan again
    // exactly.
    bool is_pre_filter = false;
    if (!literal_) {
      is_pre_filter = isPcre(exp.exp_);
      unsigned int local_flag = exp.flag_;
      if (is_pre_filter) {
        // the HS_FLAG_PREFILTER flag can't be used in combination whit HS_FLAG_SOM_LEFTMOST.
        local_flag &= ~HS_FLAG_SOM_LEFTMOST;
        local_flag |= HS_FLAG_PREFILTER;
      }
    }

    exprs_.emplace_back(std::move(exp.exp_));
    flags_.emplace_back(exp.flag_);
    ids_.emplace_back(static_cast<unsigned int>(ids_.size()));
    real_ids_.emplace_back(exp.id_);
    logic_id_map_.insert({exp.id_, ids_.size()});
    if (is_pre_filter) {
      pcre_pattern_list_.add(exprs_.back(), (exp.flag_ & HS_FLAG_CASELESS) != 0, false, exp.id_);
    }

    if (init_raw_data) {
      initRawData();
    }
  }

  assert(exprs_.size() == flags_.size());
  assert(exprs_.size() == ids_.size());
  assert(exprs_.size() == logic_id_map_.size());
  assert(exprs_.size() == real_ids_.size());
}

size_t ExpressionList::size() const { return exprs_.size(); }

void ExpressionList::clear() {
  expr_pointers_.clear();
  expr_lens_.clear();
  exprs_.clear();
  flags_.clear();
  ids_.clear();
  real_ids_.clear();
  logic_id_map_.clear();
  pcre_pattern_list_.clear();
}

uint64_t ExpressionList::getRealId(unsigned int id) const {
  assert(id < real_ids_.size());
  if (id < real_ids_.size()) {
    return real_ids_.at(id);
  }

  return -1;
}
} // namespace Hyperscan
} // namespace Common
} // namespace Wge