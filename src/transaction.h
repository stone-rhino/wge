#pragma once

#include <memory>
#include <string_view>

#include "http_extractor.h"

namespace SrSecurity {
class Engine;
class Transaction {
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

private:
  HttpExtractor extractor_;
  const Engine& engin_;
};

using TransactionPtr = std::unique_ptr<Transaction>;
} // namespace SrSecurity