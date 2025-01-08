#pragma once

#include <any>
#include <memory>
#include <string_view>
#include <unordered_map>

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
  template <class T> const T& getVariable(const std::string& name, const T& default_value) const {
    auto iter = tx_.find(name);
    if (iter == tx_.end()) {
      return default_value;
    }

    const T* value = nullptr;
    try {
      value = &(std::any_cast<const T&>(iter->second));
    } catch (const std::bad_any_cast&) {
    }

    return value ? *value : default_value;
  }

private:
  HttpExtractor extractor_;
  const Engine& engin_;
  std::unordered_map<std::string, std::any> tx_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity