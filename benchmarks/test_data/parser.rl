#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <algorithm>

#ifndef ENABLE_DEBUG_LOG
#define ENABLE_DEBUG_LOG 0
#endif

#if ENABLE_DEBUG_LOG
#include <iostream>
#include <format>
#define DEBUG_LOG(x) std::cout << x << std::endl;
#else
#define DEBUG_LOG(x)
#endif

// The request and response are surrounded by {{ and }}.
// In the block, the request and response are separated by a line of dashes.
%%{
  machine parser;

  action skip {
    DEBUG_LOG(std::format("skip:{}", std::string_view(ts, te - ts)));
  }

  action block_start {
    DEBUG_LOG("block_start");
    http_info.clear();
    header_name_buffer = {};
    header_value_buffer = {};
    lower_header_name_buffer.clear();
    body_len = 0;
  }

  action block_end {
    DEBUG_LOG("block_end");
    result.push_back(http_info); 
  }

  action request_method_start {
    http_info.request_method_ = std::string_view(p, 0);
    DEBUG_LOG("request_method_start");
  }

  action request_method_end {
    http_info.request_method_ = std::string_view(http_info.request_method_.data(), p - http_info.request_method_.data());
    DEBUG_LOG(std::format("request_method_end:{}",http_info.request_method_));
  }

  action request_uri_start {
    http_info.request_uri_ = std::string_view(p, 0);
    DEBUG_LOG("request_uri_start");
  }

  action request_uri_end {
    http_info.request_uri_ = std::string_view(http_info.request_uri_.data(), p - http_info.request_uri_.data());
    DEBUG_LOG(std::format("request_uri_end:{}",http_info.request_uri_));
  }

  action request_version_start {
    http_info.request_version_ = std::string_view(p, 0);
    DEBUG_LOG("request_version_start");
  }

  action request_version_end {
    http_info.request_version_ = std::string_view(http_info.request_version_.data(), p - http_info.request_version_.data());
    DEBUG_LOG(std::format("request_version_end:{}",http_info.request_version_));
  }

  action request_header_name_start {
    header_name_buffer = std::string_view(p, 0);
    DEBUG_LOG("request_header_name_start");
  }

  action request_header_name_end {
    header_name_buffer = std::string_view(header_name_buffer.data(), p - header_name_buffer.data());
    lower_header_name_buffer.resize(header_name_buffer.size());
    std::transform(header_name_buffer.begin(), header_name_buffer.end(), lower_header_name_buffer.begin(), ::tolower);
    DEBUG_LOG(std::format("request_header_name_end:{}",lower_header_name_buffer));
  }

  action request_header_value_start {
    header_value_buffer = std::string_view(p, 0);
    DEBUG_LOG(std::format("request_header_value_start"));
  }

  action request_header_value_end {
    header_value_buffer = std::string_view(header_value_buffer.data(), p - header_value_buffer.data());
    http_info.request_headers_.emplace(lower_header_name_buffer, header_value_buffer);
    if(lower_header_name_buffer == "content-length") {
      body_len = std::stoul(std::string(header_value_buffer));
    }
    DEBUG_LOG(std::format("request_header_value_end:{}",header_value_buffer));
  }

  action request_body {
    if(body_len > 0 && p + body_len <= eof) {
      http_info.request_body_ = std::string_view(p, body_len);
      p += body_len;
      DEBUG_LOG(std::format("request_body:{}",http_info.request_body_.back()));
    } else {
      p--;
      DEBUG_LOG(std::format("There is no request body"));
    }

    fhold;
  }

  # response
  action response_protocol_start {
    http_info.response_protocol_ = std::string_view(p, 0);
    DEBUG_LOG("response_protocol");
  }

  action response_protocol_end {
    http_info.response_protocol_ = std::string_view(http_info.response_protocol_.data(), p - http_info.response_protocol_.data());
    DEBUG_LOG(std::format("response_protocol_end:{}",http_info.response_protocol_));
  }

  action response_status_code_start {
    http_info.response_status_code_ = std::string_view(p, 0);
    DEBUG_LOG("response_status");
  }

  action response_status_code_end {
    http_info.response_status_code_ = std::string_view(http_info.response_status_code_.data(), p - http_info.response_status_code_.data());
    DEBUG_LOG(std::format("response_status_end:{}",http_info.response_status_code_));
  }

  action response_header_name_start {
    header_name_buffer = std::string_view(p, 0);
    DEBUG_LOG("response_header_name_start");
  }

  action response_header_name_end {
    header_name_buffer = std::string_view(header_name_buffer.data(), p - header_name_buffer.data());
    lower_header_name_buffer.resize(header_name_buffer.size());
    std::transform(header_name_buffer.begin(), header_name_buffer.end(), lower_header_name_buffer.begin(), ::tolower);
    DEBUG_LOG(std::format("response_header_name_end:{}",header_name_buffer));
  }

  action response_header_value_start {
    header_value_buffer = std::string_view(p, 0);
    DEBUG_LOG(std::format("response_header_value_start"));
  }

  action response_header_value_end {
    header_value_buffer = std::string_view(header_value_buffer.data(), p - header_value_buffer.data());
    http_info.response_headers_.emplace(lower_header_name_buffer, header_value_buffer);
    if(lower_header_name_buffer == "content-length") {
      body_len = std::stoul(std::string(header_value_buffer));
    }
    DEBUG_LOG(std::format("response_header_value_end:{}",header_value_buffer));
  }

  action response_body {
    if(body_len > 0 && p + body_len <= eof) {
      http_info.response_body_ = std::string_view(p, body_len);
      p += body_len;
      DEBUG_LOG(std::format("response_body:{}",http_info.response_body_.back()));
    } else {
      p--;
      DEBUG_LOG(std::format("There is no response body"));
    }

    fhold;
  }  

  BLOCK_START = '{{' >block_start;
  BLOCK_END = '}}' >block_end;
  CRLF = '\r'? '\n';

  REQUEST_METHOD = ('GET' | 'POST' | 'PUT' | 'DELETE' | 'HEAD' | 'OPTIONS' | 'PATCH') >request_method_start %request_method_end;
  REQUEST_URI = [^ \t\r\n]+ >request_uri_start %request_uri_end;
  REQUEST_VERSION = ([0-9] '.' [0-9]) >request_version_start %request_version_end;
  REQUEST_LINE = REQUEST_METHOD ' ' REQUEST_URI ' ' 'HTTP/' REQUEST_VERSION;
  REQUEST_HEADER_NAME = [^ \t\r\n:]+ >request_header_name_start %request_header_name_end;
  REQUEST_HEADER_VALUE = [^\r\n]+ >request_header_value_start %request_header_value_end;
  REQUEST_BODY = any >request_body;
  REQUEST = 
    REQUEST_LINE CRLF
    (REQUEST_HEADER_NAME ':' [ \t]* REQUEST_HEADER_VALUE CRLF)+
    CRLF
    REQUEST_BODY;

  RESPONSE_PROTOCOL = 'HTTP/1.1' >response_protocol_start %response_protocol_end;
  RESPONSE_STATUS_CODE = [0-9]{3} >response_status_code_start %response_status_code_end;
  RESPONSE_STATUS_TEXT = [^ \t\r\n]+;
  RESPONSE_LINE = RESPONSE_PROTOCOL ' ' RESPONSE_STATUS_CODE ' ' RESPONSE_STATUS_TEXT;
  RESPONSE_HEADER_NAME = [^ \t\r\n:]+ >response_header_name_start %response_header_name_end;
  RESPONSE_HEADER_VALUE = [^\r\n]+ >response_header_value_start %response_header_value_end;
  RESPONSE_BODY = any >response_body;
  RESPONSE = 
    RESPONSE_LINE CRLF
    (RESPONSE_HEADER_NAME ':' [ \t]* RESPONSE_HEADER_VALUE CRLF)+
    CRLF
    RESPONSE_BODY;

  BLOCK = 
    BLOCK_START
    CRLF
    REQUEST
    CRLF
    RESPONSE
    CRLF
    BLOCK_END;

  main := |*
    BLOCK;
    '#' [^\r\n]* [\r\n]+ => skip;
    [ \t\r\n]+ => skip;
  *|;
}%%

%% write data;

static void parse(const std::string& input, std::vector<HttpInfo>& result) {
  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;
  int top = 0;
  int stack[16];

  HttpInfo http_info;
  std::string_view header_name_buffer;
  std::string_view header_value_buffer;
  std::string lower_header_name_buffer;
  size_t body_len = 0;

  %% write init;
  %% write exec;
}

#undef DEBUG_LOG