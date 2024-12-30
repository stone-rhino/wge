#pragma once

#include <memory>
#include <vector>

#include "rule.h"
#include "transaction.h"

namespace SrSecurity::Antlr4 {
class Parser;
}

namespace SrSecurity {
class Engine {
public:
  Engine();

public:
  /**
   * Load the rule set from a file
   * @param file_path Supports relative and absolute path
   * @result An error string is returned if fails, an empty string is returned otherwise
   */
  std::string loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive Configuration directive
   * @result An error string is returned if fails, an empty string is returned otherwise
   */
  std::string load(const std::string& directive);

  /**
   * This method initializes some important variables, such as hyperscan database and so on
   * @note Must call once before call makeTransaction, and only once in the life of the
   * instance.
   */
  void preEvaluateRules();

public:
  /**
   * Make a transaction to evaluate rules.
   * @return Pointer of transaction
   * @note Must call preEvaluateRules once before call this
   */
  TransactionPtr makeTransaction() const;

private:
  void initValidRules();

private:
  std::unique_ptr<Antlr4::Parser> parser_;
  std::vector<std::unique_ptr<Rule>> rules_pool_;
  constexpr static size_t phase_total_ = 5;
  std::array<std::vector<Rule*>, phase_total_> valid_rules_;
};
} // namespace SrSecurity