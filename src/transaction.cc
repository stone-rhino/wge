#include "transaction.h"

#include "common/likely.h"

namespace SrSecurity {
Transaction::Transaction(const Engine& engin) : engin_(engin) {}

void Transaction::processUri(UriExtractor uri_extractor, Result& result) {
  extractor_.uri_extractor_ = std::move(uri_extractor);
  std::string_view uri = extractor_.uri_extractor_();
  if (unlikely(uri.empty())) {
    return;
  }
}

void Transaction::processRequestHeader(HeaderExtractor header_extractor, Result& result) {
  extractor_.request_header_extractor_ = std::move(header_extractor);
}

void Transaction::processRequestBody(BodyExtractor body_extractor, Result& result) {
  extractor_.reqeust_body_extractor_ = std::move(body_extractor);
}

void Transaction::processResponseHeader(HeaderExtractor header_extractor, Result& result) {
  extractor_.response_header_extractor_ = std::move(header_extractor);
}

void Transaction::processResponseBody(BodyExtractor body_extractor, Result& result) {
  extractor_.response_body_extractor_ = std::move(body_extractor);
}

} // namespace SrSecurity