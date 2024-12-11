#include "transaction.h"

#include "common/likely.h"

namespace SrSecurity {
Transaction::Transaction(const Engine& engin) : engin_(engin) {}

void Transaction::processUri(UriExtractor uri_extractor, Result& result) {
  uri_extractor_ = std::move(uri_extractor);
  std::string_view uri = uri_extractor_();
  if (unlikely(uri.empty())) {
    return;
  }
}

void Transaction::processRequestHeader(HeaderExtractor header_extractor, Result& result) {
  request_header_extractor_ = std::move(header_extractor);
}

void Transaction::processRequestBody(BodyExtractor body_extractor, Result& result) {
  reqeust_body_extractor_ = std::move(body_extractor);
}

void Transaction::processResponseHeader(HeaderExtractor header_extractor, Result& result) {
  response_header_extractor_ = std::move(header_extractor);
}

void Transaction::processResponseBody(BodyExtractor body_extractor, Result& result) {
  response_body_extractor_ = std::move(body_extractor);
}

} // namespace SrSecurity