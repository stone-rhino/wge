#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "audit_log_config.h"
#include "engine_config.h"
#include "http_extractor.h"

namespace SrSecurity {
class Engine;
class Transaction final {
  friend class Engine;

protected:
  Transaction(const Engine& engin);

public:
  void processConnection(ConnectionExtractor conn_extractor);
  void processUri(UriExtractor uri_extractor);
  void processRequestHeaders(HeaderExtractor header_extractor);
  void processRequestBody(BodyExtractor body_extractor);
  void processResponseHeaders(HeaderExtractor header_extractor);
  void processResponseBody(BodyExtractor body_extractor);

public:
  void createVariable(std::string&& name, int value = 1);
  void createVariable(std::string&& name, std::string&& value);
  void removeVariable(const std::string& name);
  void increaseVariable(const std::string& name, int value = 1);
  const std::string* getVariable(const std::string& name) const;
  std::string* getVariable(const std::string& name);
  int getVariableInt(const std::string& name) const;
  void setMatched(size_t index, const std::string_view& value);
  const std::string_view* getMatched(size_t index) const;

private:
  HttpExtractor extractor_;
  const Engine& engin_;
  std::unordered_map<std::string, std::string> tx_;
  std::array<std::string_view, 100> matched_;

  // ctl
private:
  std::optional<AuditLogConfig::AuditEngine> audit_engine_;
  std::optional<AuditLogConfig::AuditLogPart> audit_log_part_;
  std::optional<EngineConfig::Option> request_body_access_;
  std::optional<EngineConfig::Option> rule_engine_;
  std::vector<uint64_t> rule_remove_by_id_;
  std::vector<std::string> rule_remove_by_tag_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity