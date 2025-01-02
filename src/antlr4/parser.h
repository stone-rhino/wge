#pragma once

#include <array>
#include <list>
#include <memory>
#include <string>
#include <unordered_set>

#include "../rule.h"

namespace SrSecurity::Antlr4 {

/**
 * SecLang parser
 */
class Parser {
public:
  /**
   * Load the rule set from a file
   * @param file_path supports relative and absolute path
   * @result an error string is returned if fails, an empty string is returned otherwise
   */
  std::string loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive Configuration directive
   * @result An error string is returned if fails, an empty string is returned otherwise
   */
  std::string load(const std::string& directive);

public:
  // Engine configuration
  struct EngineConfig {
    enum class Option { On, Off, DetectionOnly };
    // SecRequestBodyAccess
    // Configures whether request bodies will be buffered and processed by ModSecurity.
    Option is_request_body_access_{Option::Off};

    // SecResponseBodyAccess
    // Configures whether response bodies are to be buffered.
    Option is_response_body_access_{Option::Off};

    // SecRuleEngine
    // Configures the rules engine.
    Option is_rule_engine_{Option::Off};

    // SecTmpSaveUploadedFiles
    // Configures whether or not files uploaded via a multipart POST request will be temporarily
    // saved to the file system.
    Option is_tmp_save_uploaded_files_{Option::Off};

    // SecUploadKeepFiles
    // Configures whether or not the intercepted files will be kept after transaction is processed.
    Option is_upload_keep_files_{Option::Off};

    // SecXmlExternalEntity
    // Enable or Disable the loading process of xml external entity. Loading external entity without
    // correct verifying process can lead to a security issue.
    Option is_xml_external_entity_{Option::Off};
  };

public:
  EngineConfig& engineConfig() { return engine_config_; }
  const EngineConfig& engineConfig() const { return engine_config_; }
  std::list<std::unique_ptr<Rule>>& rules() { return rules_; }
  const std::list<std::unique_ptr<Rule>>& rules() const { return rules_; }

public:
  static const std::unordered_map<
      std::string,
      std::function<std::unique_ptr<Variable::VariableBase>(std::string&&, bool, bool)>>&
  getVariableFactory() {
    return variable_factory_;
  }

  static const std::unordered_map<
      std::string,
      std::function<std::unique_ptr<Operator::OperatorBase>(std::string&&, std::string&&)>>
  getOperatorFactory() {
    return operator_factory_;
  }

  static const std::unordered_map<std::string, std::function<void(Rule&, std::string&&)>>
  getActionFactory() {
    return action_factory_;
  }

private:
  EngineConfig engine_config_;
  std::list<std::unique_ptr<Rule>> rules_;

private:
  static std::unordered_map<std::string, std::function<std::unique_ptr<Variable::VariableBase>(
                                             std::string&&, bool, bool)>>
      variable_factory_;
  static std::unordered_map<std::string, std::function<std::unique_ptr<Operator::OperatorBase>(
                                             std::string&&, std::string&&)>>
      operator_factory_;
  static std::unordered_map<std::string, std::function<void(Rule&, std::string&&)>> action_factory_;
};
} // namespace SrSecurity::Antlr4
