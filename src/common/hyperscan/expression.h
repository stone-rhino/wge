#pragma once

#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <stdint.h>

#include "../pcre/pattern.h"

namespace SrSecurity {
namespace Common {
namespace Hyperscan {
struct Expression {
  std::string exp_;
  unsigned int flag_;
  uint64_t id_;
};

class ExpressionList {
public:
  /**
   * Constructor
   * @param literal whether the expression is literal. if true, when call load() and add() the
   * pattern will be literal
   */
  ExpressionList(bool literal) : literal_(literal) {}

public:
  /**
   * Load patterns from the specified file.
   * Each line in the file is a pattern.
   * @param ifs the file stream
   * @param utf8 whether the pattern is utf8
   * @param som_leftmost whether the pattern is som_leftmost
   * @param multi_line whether the pattern is multi_line
   * @note:
   * Starts with charactor '#' means comment this line
   * Starts with double charactor '#' means comment all of follow
   */
  bool load(std::ifstream& ifs, bool utf8, bool som_leftmost, bool multi_line);

  /**
   * Add a pattern to the list
   * @param exp the pattern
   * @param init_raw_data whether init raw data now. we must specify true at last call add() for the
   * list to get raw data.
   */
  void add(Expression&& exp, bool init_raw_data);
  size_t size() const;
  void clear();
  bool literal() const { return literal_; }

  // Get raw data of array
  const char** exprRawData() {
    return expr_pointers_.data();
  }
  const size_t* exprLenRawData() {
    return expr_lens_.data();
  }
  const unsigned int* flagsRawData() { return flags_.data(); }
  const unsigned int* idsRawData() { return ids_.data(); }

  uint64_t getRealId(unsigned int id) const;
  const Pcre::PatternList& getPcrePatternList() const { return pcre_pattern_list_; }

  static bool isPcre(const std::string& expression) {
    static std::regex is_pcre_pattern(R"(.*(?:\(\?=|\(\?!|\(\?<=|\(\?<!|\(\?>|\\\d|[?*+}]\+).*)");
    return std::regex_match(expression, is_pcre_pattern);
  }

private:
  void initRawData() {
    expr_pointers_.reserve(exprs_.size());
    expr_lens_.reserve(exprs_.size());
    for (const auto& expr : exprs_) {
      expr_pointers_.emplace_back(expr.c_str());
      expr_lens_.emplace_back(expr.length());
    }
  }

private:
  bool literal_{false};
  std::vector<const char*> expr_pointers_;
  std::vector<size_t> expr_lens_;
  std::vector<std::string> exprs_;
  std::vector<unsigned int> flags_;
  std::vector<unsigned int> ids_;
  std::vector<uint64_t> real_ids_;
  std::unordered_map<uint64_t, unsigned int> logic_id_map_;
  Pcre::PatternList pcre_pattern_list_;
};
} // namespace Hyperscan
} // namespace Common
} // namespace SrSecurity