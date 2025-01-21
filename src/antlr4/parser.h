#pragma once

#include <array>
#include <expected>
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
   * @result An error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> loadFromFile(const std::string& file_path);

  /**
   * Load the rule set from a configuration directive
   * @param directive Configuration directive
   * @result An error string is returned if fails, and returned true otherwise
   */
  std::expected<bool, std::string> load(const std::string& directive);

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

  struct AuditLogConfig {
    enum class AuditEngine {
      // log all transactions
      On,
      // do not log any transactions
      Off,
      // only the log transactions that have triggered a warning or an error, or have a status code
      // that is considered to be relevant (as determined by the SecAuditLogRelevantStatus
      // directive)
      RelevantOnly
    };

    enum class AuditLogType { Serial, Concurrent, Https };

    enum class AuditFormat { Json, Native };

    enum class AuditLogPart {
      A = 0,
      Headers = A,
      B = 1,
      RequestHeaders = B,
      C = 2,
      RequestBody = C,
      D = 3,
      IntermediaryResponseHeaders = D,
      E = 4,
      IntermediaryResponseBody = E,
      F = 5,
      FinalResponseHeaders = F,
      G = 6,
      FinalResponseBody = G,
      H = 7,
      Trailer = H,
      I = 8,
      J = 9,
      Uploaded = J,
      K = 10,
      Z = 11,
      FinalBoundary = Z,
      End = 12
    };

    // SecAuditEngine
    // Configures the audit logging engine.
    AuditEngine audit_engine_;

    // SecAuditLogType
    // Configures the type of audit logging mechanism to be used.
    AuditLogType audit_log_type_;

    // SecAuditLog
    // Defines the path to the main audit log file (serial logging format), or the concurrent
    // logging index file (concurrent logging format), or the url (HTTPS).
    std::string log_path_;

    // SecAuditLogStorageDir
    // Configures the directory where concurrent audit log entries are to be stored.
    std::string storage_dir_;

    // SecAuditLog2
    // Defines the path to the secondary audit log index file when concurrent logging is enabled.
    // See SecAuditLog for more details.
    std::string log_path2_;

    // SecAuditLogDirMode
    // Configures the mode (permissions) of any directories created for the concurrent audit logs,
    // using an octal mode value as parameter (as used in chmod).
    int dir_mode_;

    // SecAuditLogFileMode
    // Configures the mode (permissions) of any files created for concurrent audit logs using an
    // octal mode (as used in chmod). See SecAuditLogDirMode for controlling the mode of created
    // audit log directories.
    int file_mode_;

    // SecAuditLogFormat
    // Select the output format of the AuditLogs. The format can be either the native AuditLogs
    // format or JSON.
    AuditFormat format_;

    // SecAuditLogParts
    // Defines which parts of each transaction are going to be recorded in the audit log. Each part
    // is assigned a single letter; when a letter appears in the list then the equivalent part will
    // be recorded. See below for the list of all parts.
    bool log_parts_[static_cast<int>(AuditLogPart::End)];

    // SecAuditLogRelevantStatus
    // Configures which response status code is to be considered relevant for the purpose of audit
    // logging.
    std::string relevant_status_regex_;

    // SecComponentSignature
    // Appends component signature to the ModSecurity signature.
    std::string component_signature_;
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
  std::list<std::unique_ptr<Rule>>::iterator secRule();
  void secRuleRemoveById(uint64_t id);
  void secRuleRemoveByMsg(const std::string& msg);
  void secRuleRemoveByTag(const std::string& tag);
  void secRuleUpdateTargetById(uint64_t id);
  void secRuleUpdateTargetByMsg(const std::string& msg);
  void secRuleUpdateTargetByTag(const std::string& tag);

  // Audit log configurations
  void secAuditEngine(AuditLogConfig::AuditEngine option);
  void secAuditLog(std::string&& path);
  void secAuditLog2(std::string&& path);
  void secAuditLogDirMode(int mode);
  void secAuditLogFormat(AuditLogConfig::AuditFormat format);
  void secAuditLogFileMode(int mode);
  void secAuditLogParts(const std::string& parts);
  void secAuditLogRelevantStatus(std::string&& pattern);
  void secAuditLogStorageDir(std::string&& dir);
  void secAuditLogType(AuditLogConfig::AuditLogType type);
  void secComponentSignature(std::string&& signature);

public:
  const EngineConfig& engineConfig() const { return engine_config_; }
  const std::list<std::unique_ptr<Rule>>& rules() const { return rules_; }
  const AuditLogConfig& auditLogConfig() const { return audit_log_config_; }
  void removeBackRule();
  void setRuleIdIndex(std::list<std::unique_ptr<Rule>>::iterator iter);
  void clearRuleIdIndex(std::list<std::unique_ptr<Rule>>::iterator iter);
  void setRuleMsgIndex(std::list<std::unique_ptr<Rule>>::iterator iter);
  void clearRuleMsgIndex(std::list<std::unique_ptr<Rule>>::iterator iter);
  void setRuleTagIndex(std::list<std::unique_ptr<Rule>>::iterator iter,
                       const std::string_view& tag);
  void clearRuleTagIndex(std::list<std::unique_ptr<Rule>>::iterator iter);
  std::list<std::unique_ptr<Rule>>::iterator findRuleById(uint64_t id);
  std::pair<std::unordered_multimap<std::string_view,
                                    std::list<std::unique_ptr<Rule>>::iterator>::iterator,
            std::unordered_multimap<std::string_view,
                                    std::list<std::unique_ptr<Rule>>::iterator>::iterator>
  findRuleByMsg(const std::string& msg);
  std::pair<std::unordered_multimap<std::string_view,
                                    std::list<std::unique_ptr<Rule>>::iterator>::iterator,
            std::unordered_multimap<std::string_view,
                                    std::list<std::unique_ptr<Rule>>::iterator>::iterator>
  findRuleByTag(const std::string& tag);

private:
  EngineConfig engine_config_;
  std::list<std::unique_ptr<Rule>> rules_;
  std::unordered_map<uint64_t, std::list<std::unique_ptr<Rule>>::iterator> rules_index_id_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_msg_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_tag_;
  AuditLogConfig audit_log_config_;
};
} // namespace SrSecurity::Antlr4
