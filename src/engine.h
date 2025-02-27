#pragma once

#include <expected>
#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "common/log.h"
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
   * @param file_path the rule(SecLang) file path. support absolute path and relative path
   * @result an error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive configuration directive. such as "SecRuleEngine On"
   * @result an error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> load(const std::string& directive);

  /**
   * Initialize the engine
   * @param level the log level
   * @param log_file the log file path. If it is empty, the log will be output to the console
   * @note must call once before call makeTransaction, and only once in the life of the engine
   * instance.
   */
  void init(spdlog::level::level_enum level = spdlog::level::info,
            const std::string& log_file = "");

  /**
   * Get default actions
   * @param phase specify the phase of the default actions, the valid range is 1-5.
   * @return vector of default actions
   */
  const Rule* defaultActions(int phase) const;

  /**
   * Get rules
   * @param phase specify the phase of rule, the valid range is 1-5.
   * @return vector of rules
   */
  const std::vector<const Rule*>& rules(int phase) const;

public:
  /**
   * Make a transaction to evaluate rules.
   * @return pointer of transaction
   * @note must call init before call this method
   */
  TransactionPtr makeTransaction() const;

  /**
   * Get the parser
   * @return reference of parser
   * FIXME(zhouyu, 2025-02-27): The parser should be private, and provide some method wrapper to
   * access the parser.
   */
  const Antlr4::Parser& parser() const { return *parser_; }

  /**
   * Find the rule by id
   * @param id the rule id
   * @return pointer of rule if found, and nullptr otherwise
   */
  const Rule* findRuleById(uint64_t id) const;

  /**
   * Find the rule by tag
   * @param tag the rule tag
   */
  void findRuleByTag(const std::string& tag,
                     std::array<std::unordered_set<const Rule*>, PHASE_TOTAL>& rule_set) const;

  std::optional<const std::vector<const Rule*>::iterator> marker(const std::string& name,
                                                                 int phase) const;

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
  // The parser is used to parse the SecLang rule set.
  std::shared_ptr<Antlr4::Parser> parser_;

  // Default actions defined in the SecDefaultAction directive
  // The default action is executed in the same phase as the rule that is matched and before
  // evaluating the rule's actions.
  std::array<const Rule*, PHASE_TOTAL> default_actions_{nullptr};

  // Even though the parser parses the rule set and stores the parsed rules, the parser  is not used
  // to evaluate the rules. Because each phase has a separate rule set, for performance reasons, we
  // store the each phase's rule pointers in an array.
  std::array<std::vector<const Rule*>, PHASE_TOTAL> rules_;

  std::unordered_map<std::string, Marker&> markers_;
  PersistentStorage::Storage storage_;
};
} // namespace SrSecurity