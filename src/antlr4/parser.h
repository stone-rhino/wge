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
#pragma once

#include <array>
#include <expected>
#include <list>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>

#include "../config.h"
#include "../marker.h"
#include "../rule.h"

namespace SrSecurity::Antlr4 {

/**
 * SecLang parser
 */
class Parser {
public:
  Parser();

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
  void secResponseBodyMimeType(const std::vector<std::string>& mime_types);
  void secResponseBodyMimeTypeClear();
  void secResponseBodyAccess(EngineConfig::Option option);
  void secRuleEngine(EngineConfig::Option option);
  void secTmpSaveUploadedFiles(EngineConfig::Option option);
  void secUploadFileLimit(uint32_t limit_count);
  void secUploadKeepFiles(EngineConfig::Option option);
  void secXmlExternalEntity(EngineConfig::Option option);
  void secRequestBodyLimit(uint64_t limit_bytes);
  void secRequestBodyNoFilesLimit(uint64_t limit_bytes);
  void secRequestBodyJsonDepthLimit(uint64_t limit);
  void secRequsetBodyLimitAction(EngineConfig::BodyLimitAction action);
  void secResponseBodyLimit(uint64_t limit_bytes);
  void secResponseBodyLimitAction(EngineConfig::BodyLimitAction action);
  void secArgumentsLimit(uint32_t limit_bytes);
  void secArgumentSeparator(char separator);
  void secUnicodeMapFile(std::string&& file_path, uint32_t code_point);
  void secPcreMatchLimit(uint32_t limit);

  // Engine action
  std::list<std::unique_ptr<Rule>>::iterator secAction(int line);

  // Rule directives
  std::list<std::unique_ptr<Rule>>::iterator secRule(int line);
  void secRuleRemoveById(uint64_t id);
  void secRuleRemoveByMsg(const std::string& msg);
  void secRuleRemoveByTag(const std::string& tag);
  void secMarker(std::string&& name);
  std::list<std::unique_ptr<Rule>>::iterator secDefaultAction(int line);

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
  const std::list<std::unique_ptr<Rule>>& defaultActions() const { return default_actions_; }
  const std::list<std::unique_ptr<Rule>>& rules() const { return rules_; }
  const std::list<Marker>& markers() const { return makers_; }
  const AuditLogConfig& auditLogConfig() const { return audit_log_config_; }
  void removeBackRule();
  void removeBackDefaultAction();
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
  std::string_view currLoadFile() const { return curr_load_file_; }

  size_t getTxVariableIndexSize() const { return tx_variable_index_.size(); }
  std::optional<size_t> getTxVariableIndex(const std::string& name, bool force);
  std::string_view getTxVariableIndexReverse(size_t index) const;

private:
  EngineConfig engine_config_;
  AuditLogConfig audit_log_config_;
  std::list<std::unique_ptr<Rule>> default_actions_;
  std::list<std::unique_ptr<Rule>> rules_;
  std::unordered_map<uint64_t, std::list<std::unique_ptr<Rule>>::iterator> rules_index_id_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_msg_;
  std::unordered_multimap<std::string_view, std::list<std::unique_ptr<Rule>>::iterator>
      rules_index_tag_;
  std::list<Marker> makers_;

  std::set<std::string> loaded_file_paths_;
  std::string_view curr_load_file_;

  // Used to store the tx variable index of the vector tx_vec_.
  std::unordered_map<std::string, size_t> tx_variable_index_;
  std::vector<std::string_view> tx_variable_index_reverse_;
};
} // namespace SrSecurity::Antlr4
