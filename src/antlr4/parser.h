#pragma once

#include <array>
#include <expected>
#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "../audit_log_config.h"
#include "../engine_config.h"
#include "../marker.h"
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

  // Modsecurity configuration directive
public:
  // Engine configurations
  void secRequestBodyAccess(EngineConfig::Option option);
  void secResponseBodyAccess(EngineConfig::Option option);
  void secRuleEngine(EngineConfig::Option option);
  void secTmpSaveUploadedFiles(EngineConfig::Option option);
  void secUploadKeepFiles(EngineConfig::Option option);
  void secXmlExternalEntity(EngineConfig::Option option);

  // Engine action
  std::list<std::unique_ptr<Rule>>::iterator secAction();

  // Rule directives
  std::list<std::unique_ptr<Rule>>::iterator secRule();
  void secRuleRemoveById(uint64_t id);
  void secRuleRemoveByMsg(const std::string& msg);
  void secRuleRemoveByTag(const std::string& tag);
  void secMarker(std::string&& name);

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
  const std::list<Marker>& markers() const { return makers_; }
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
  AuditLogConfig audit_log_config_;
  std::list<std::unique_ptr<Rule>> rules_;
  std::unordered_map<uint64_t, std::list<std::unique_ptr<Rule>>::iterator> rules_index_id_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_msg_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_tag_;
  std::list<Marker> makers_;
};
} // namespace SrSecurity::Antlr4
