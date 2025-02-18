#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "marker.h"
#include "persistent_storage/storage.h"
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
   * @result An error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive Configuration directive
   * @result An error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> load(const std::string& directive);

  /**
   * This method initializes some important variables, such as rules chain per phase and hyperscan
   * database and so on
   * @note Must call once before call makeTransaction, and only once in the life of the engine
   * instance.
   */
  void init();

  /**
   * Get default actions
   * @param phase Specify the phase of the default actions, the valid range is 1-5.
   * @return vector of default actions
   */
  const std::vector<const Rule*>& defaultActions(int phase) const;

  /**
   * Get rules
   * @param phase Specify the phase of rule, the valid range is 1-5.
   * @return vector of rules
   */
  const std::vector<const Rule*>& rules(int phase) const;

public:
  /**
   * Make a transaction to evaluate rules.
   * @return Pointer of transaction
   * @note Must call init once before call this
   */
  TransactionPtr makeTransaction() const;

  /**
   * Get the parser
   * @return reference of parser
   */
  const Antlr4::Parser& parser() const { return *parser_; }

  std::optional<const std::vector<const Rule*>::iterator> marker(const std::string& name, int phase) const;

  /**
   * Get persistent storage
   * @return reference of persistent storage
   */
  PersistentStorage::Storage& storage() { return storage_; }

private:
  void initDefaultActions();
  void initRules();
  void initMakers();

private:
  constexpr static size_t phase_total_ = 5;
  std::shared_ptr<Antlr4::Parser> parser_;
  std::array<std::vector<const Rule*>, phase_total_> default_actions_;
  std::array<std::vector<const Rule*>, phase_total_> rules_;
  std::unordered_map<std::string, Marker&> markers_;
  PersistentStorage::Storage storage_;
};
} // namespace SrSecurity