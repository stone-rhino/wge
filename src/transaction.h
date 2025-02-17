#pragma once

#include <memory>
#include <optional>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "config.h"
#include "http_extractor.h"

namespace SrSecurity {
class Engine;
class Transaction final {
  friend class Engine;

protected:
  Transaction(const Engine& engin);

public:
  /**
   * Process the connection info.
   * @param conn_extractor the connection info extractor.
   */
  void processConnection(ConnectionExtractor conn_extractor);

  /**
   * Process the uri info.
   * @param uri_extractor the uri info extractor.
   */
  void processUri(UriExtractor uri_extractor);

  /**
   * Process the request headers.
   * @param header_extractor the request headers extractor.
   */
  void processRequestHeaders(HeaderExtractor header_extractor);

  /**
   * Process the request body.
   * @param body_extractor the request body extractor.
   */
  void processRequestBody(BodyExtractor body_extractor);

  /**
   * Process the response headers.
   * @param header_extractor the response headers extractor.
   */
  void processResponseHeaders(HeaderExtractor header_extractor);

  /**
   * Process the response body.
   * @param body_extractor the response body extractor.
   */
  void processResponseBody(BodyExtractor body_extractor);

public:
  /**
   * Create a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the int value of the variable.
   */
  void createVariable(std::string&& name, int value = 1);

  /**
   * Create a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the string value of the variable.
   */
  void createVariable(std::string&& name, std::string&& value);

  /**
   * Remove a variable from the transient transaction collection
   * @param name the name of the variable.
   */
  void removeVariable(const std::string& name);

  /**
   * Increase the value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the int value to increase.
   */
  void increaseVariable(const std::string& name, int value = 1);

  /**
   * Get the string value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @return the value of the variable. if the variable does not exist, return an empty string.
   */
  const std::string& getVariable(const std::string& name) const;

  /**
   * Get the int value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @return the value of the variable.if the variable does not exist, return 0.
   */
  int getVariableInt(const std::string& name) const;

  /**
   * Set the value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the string value of the variable.
   */
  void setVariable(const std::string& name, std::string&& value);

  /**
   * Set the value of a variable in the transient transaction collection
   * @param name the name of the variable.
   * @param value the int value of the variable.
   */
  void setVariableInt(const std::string& name, int value);

  /**
   * Check if the variable exists in the transient transaction collection
   * @param name the name of the variable.
   * @return true if the variable exists, false otherwise.
   */
  bool hasVariable(const std::string& name) const;

  /**
   * Set the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @param value the reference of the matched string.
   */
  void setMatched(size_t index, const std::string_view& value);

  /**
   * Get the matched string that is captured by the operator.
   * @param index the index of the matched string.the range is [0, 99].
   * @return the matched string.if the matched string does not exist, return nullptr.
   */
  const std::string_view* getMatched(size_t index) const;

  /**
   * Get the HTTP extractor.
   * @return the HTTP extractor.
   */
  const HttpExtractor& httpExtractor() const { return extractor_; }

  /**
   * Set the request body processor.
   * @param type the request body processor.
   */
  void setRequestBodyProcessor(BodyProcessorType type) { request_body_processor_ = type; }

  /**
   * Get the request body processor.
   * @return the request body processor.
   */
  BodyProcessorType getRequestBodyProcessor() const { return *request_body_processor_; }

  /**
   * Get the Unique ID of the transaction.
   * @return the Unique ID of the transaction.
   */
  const std::string& getUniqueId() const { return unique_id_; }

private:
  class RandomInitHelper {
  public:
    RandomInitHelper() { ::srand(::time(nullptr)); }
  };

  void initUniqueId();

private:
  std::string unique_id_;
  HttpExtractor extractor_;
  const Engine& engin_;
  std::unordered_map<std::string, std::string> tx_;
  std::array<std::string_view, 100> matched_;
  static const RandomInitHelper random_init_helper_;

  // ctl
private:
  std::optional<AuditLogConfig::AuditEngine> audit_engine_;
  std::optional<AuditLogConfig::AuditLogPart> audit_log_part_;
  std::optional<EngineConfig::Option> request_body_access_;
  std::optional<BodyProcessorType> request_body_processor_;
  std::optional<EngineConfig::Option> rule_engine_;
  std::vector<uint64_t> rule_remove_by_id_;
  std::vector<std::string> rule_remove_by_tag_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity