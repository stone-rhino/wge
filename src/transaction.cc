#include "transaction.h"

#include "common/try.h"
#include "engine.h"

namespace SrSecurity {
Transaction::Transaction(const Engine& engin) : engin_(engin) {}

void Transaction::processConnection(ConnectionExtractor conn_extractor) {
  extractor_.connection_extractor_ = std::move(conn_extractor);
}

void Transaction::processUri(UriExtractor uri_extractor) {
  extractor_.uri_extractor_ = std::move(uri_extractor);
}

void Transaction::processRequestHeaders(HeaderExtractor header_extractor) {
  extractor_.request_header_extractor_ = std::move(header_extractor);
  auto& rules = engin_.rules(1);
  for (Rule* rule : rules) {
    rule->evaluate(*this, extractor_);
  }
}

void Transaction::processRequestBody(BodyExtractor body_extractor) {
  extractor_.reqeust_body_extractor_ = std::move(body_extractor);
}

void Transaction::processResponseHeaders(HeaderExtractor header_extractor) {
  extractor_.response_header_extractor_ = std::move(header_extractor);
}

void Transaction::processResponseBody(BodyExtractor body_extractor) {
  extractor_.response_body_extractor_ = std::move(body_extractor);
}

void Transaction::createVariable(std::string&& name, int value) {
  auto iter = tx_.find(name);
  if (iter == tx_.end()) {
    tx_.emplace(std::move(name), std::to_string(value));
  } else {
    iter->second = std::to_string(value);
  }
}

void Transaction::createVariable(std::string&& name, std::string&& value) {
  auto iter = tx_.find(name);
  if (iter == tx_.end()) {
    tx_.emplace(std::move(name), value);
  }
}

void Transaction::removeVariable(const std::string& name) { tx_.erase(name); }

void Transaction::increaseVariable(const std::string& name, int value) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    int v = ::atol(iter->second.c_str());
    v += value;
    iter->second = std::to_string(v);
  }
}

const std::string* Transaction::getVariable(const std::string& name) const {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    return &iter->second;
  }

  return nullptr;
}

std::string* Transaction::getVariable(const std::string& name) {
  auto iter = tx_.find(name);
  if (iter != tx_.end()) {
    return &iter->second;
  }

  return nullptr;
}

int Transaction::getVariableInt(const std::string& name) const {
  const std::string* val = getVariable(name);
  if (val) {
    return ::atol(val->c_str());
  }

  return 0;
}

} // namespace SrSecurity