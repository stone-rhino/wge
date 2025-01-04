#pragma once

#include <array>
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

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

  struct VariableAttr {
    std::string full_name_;
    std::string main_name_;
    bool is_not_;
    bool is_counter_;
  };

  // Modsecurity configuration directive
public:
  // Engine configurations
  void secRequestBodyAccess(EngineConfig::Option option);
  void secResponseBodyAccess(EngineConfig::Option option);
  void secRuleEngine(EngineConfig::Option option);
  void secTmpSaveUploadedFiles(EngineConfig::Option option);
  void secUploadKeepFiles(EngineConfig::Option option);
  void secXmlExternalEntity(EngineConfig::Option option);

  // Rule directives
  void secRule(std::vector<VariableAttr>&& variable_attrs, std::string&& operator_name,
               std::string&& operator_value,
               std::unordered_multimap<std::string, std::string>&& actions);
  void secRuleRemoveById(uint64_t id);
  void secRuleRemoveByMsg(const std::string& msg);
  void secRuleRemoveByTag(const std::string& tag);

public:
  const EngineConfig& engineConfig() const { return engine_config_; }
  const std::list<std::unique_ptr<Rule>>& rules() const { return rules_; }

private:
  EngineConfig engine_config_;
  std::list<std::unique_ptr<Rule>> rules_;
  std::unordered_map<uint64_t, std::list<std::unique_ptr<Rule>>::iterator> rules_index_id_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_msg_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_tag_;

private:
  static std::unordered_map<std::string, std::function<void(Parser&, const std::string&)>>
      engine_config_factory_;
  static std::unordered_map<std::string, std::function<std::unique_ptr<Variable::VariableBase>(
                                             std::string&&, bool, bool)>>
      variable_factory_;
  static std::unordered_map<std::string, std::function<std::unique_ptr<Operator::OperatorBase>(
                                             std::string&&, std::string&&)>>
      operator_factory_;
  static std::unordered_map<std::string, std::function<void(Parser&, Rule&, std::string&&)>>
      action_factory_;
};
} // namespace SrSecurity::Antlr4
