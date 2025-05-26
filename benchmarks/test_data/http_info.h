#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

struct HttpInfo {
  std::string_view request_method_;
  std::string_view request_uri_;
  std::string_view request_version_;
  std::unordered_multimap<std::string, std::string_view> request_headers_;
  std::string_view request_body_;
  std::string_view response_protocol_;
  std::string_view response_status_code_;
  std::string_view response_status_text_;
  std::unordered_multimap<std::string, std::string_view> response_headers_;
  std::string_view response_body_;

  void clear() {
    request_method_ = {};
    request_uri_ = {};
    request_version_ = {};
    request_headers_.clear();
    request_body_ = {};
    response_protocol_ = {};
    response_status_code_ = {};
    response_status_text_ = {};
    response_headers_.clear();
    response_body_ = {};
  }
};