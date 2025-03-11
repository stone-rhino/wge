#include "expression.h"

#include <fstream>
#include <regex>

#include <hs/hs.h>

#include "../log.h"

namespace SrSecurity {
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

    if (buffer.at(0) == '#') {
      if (buffer.at(1) == '#') {
        break;
      }
      continue;
    }

    add({std::move(buffer), flag, id});
    ++id;
  }

  // We need to reinitialize the raw data
  inited_raw_data_ = false;

  return true;
}

void ExpressionList::add(Expression&& exp) {
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
      pcre_pattern_list_.add(exprs_.back(), (exp.flag_ & HS_FLAG_CASELESS) != 0, exp.id_);
    }

    // We need to reinitialize the raw data
    inited_raw_data_ = false;
  }

  assert(exprs_.size() == flags_.size());
  assert(exprs_.size() == ids_.size());
  assert(exprs_.size() == logic_id_map_.size());
  assert(exprs_.size() == real_ids_.size());
}

size_t ExpressionList::size() const { return exprs_.size(); }

void ExpressionList::clear() {
  expr_pointers_.clear();
  exprs_.clear();
  flags_.clear();
  ids_.clear();
  real_ids_.clear();
  logic_id_map_.clear();
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
} // namespace SrSecurity