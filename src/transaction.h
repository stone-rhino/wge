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
  struct Result {
    bool intervened_{false};
    std::string_view message_;
    std::string_view header_key_;
    int64_t from_{0};
    int64_t to_{0};
  };

public:
  void processUri(UriExtractor uri_extractor, Result& result);
  void processRequestHeader(HeaderExtractor header_extractor, Result& result);
  void processRequestBody(BodyExtractor body_extractor, Result& result);
  void processResponseHeader(HeaderExtractor header_extractor, Result& result);
  void processResponseBody(BodyExtractor body_extractor, Result& result);

public:
  void createVariable(std::string&& name, int value = 1);
  void createVariable(std::string&& name, std::string&& value);
  void removeVariable(const std::string& name);
  void increaseVariable(const std::string& name, int value = 1);
  const std::string* getVariable(const std::string& name) const;
  std::string* getVariable(const std::string& name);
  int getVariableInt(const std::string& name) const;

private:
  HttpExtractor extractor_;
  const Engine& engin_;
  std::unordered_map<std::string, std::string> tx_;

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