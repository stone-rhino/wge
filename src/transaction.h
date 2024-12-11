#pragma once
#include <functional>
#include <memory>
#include <string_view>

namespace SrSecurity {
class Engine;
class Transaction {
  friend class Engine;

protected:
  Transaction(const Engine& engin);

public:
  struct Result {
    bool intervention_{false};
    std::string_view message_;
    std::string_view header_key_;
    int64_t from_{0};
    int64_t to_{0};
  };

  using UriExtractor = std::function<std::string_view()>;
  using HeaderExtractor =
      std::function<void(const std::string_view& key, std::vector<std::string_view>& values)>;
  using BodyExtractor = std::function<void(std::vector<std::string_view>& body_slices)>;

public:
  void processUri(UriExtractor uri_extractor, Result& result);
  void processRequestHeader(HeaderExtractor header_extractor, Result& result);
  void processRequestBody(BodyExtractor body_extractor, Result& result);
  void processResponseHeader(HeaderExtractor header_extractor, Result& result);
  void processResponseBody(BodyExtractor body_extractor, Result& result);

private:
  UriExtractor uri_extractor_;
  HeaderExtractor request_header_extractor_;
  HeaderExtractor response_header_extractor_;
  BodyExtractor reqeust_body_extractor_;
  BodyExtractor response_body_extractor_;
  const Engine& engin_;
};

using TransactionSharedPtr = std::shared_ptr<Transaction>;
} // namespace SrSecurity