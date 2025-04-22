/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string_view>

#ifndef ENABLE_MULTI_PART_DEBUG_LOG
#define ENABLE_MULTI_PART_DEBUG_LOG 0
#endif

#if ENABLE_MULTI_PART_DEBUG_LOG
#include <iostream>
#include <format>
#define MULTI_PART_LOG(x) std::cout << x << std::endl;
#else
#define MULTI_PART_LOG(x)
#endif

%%{
  machine multipart_content_type;
  
  action error_boundary_whitespace {
    MULTI_PART_LOG("error_boundary_whitespace");
    error_code.set(MultipartStrictError::ErrorType::BoundaryWhitespace);
    fbreak;
  }

  action error_boundary_quoted {
    MULTI_PART_LOG("error_boundary_quoted");
    error_code.set(MultipartStrictError::ErrorType::BoundaryQuoted);
    fbreak;
  }

  action start_boundary {
    boundary_start = p - 2;
  }

  action end_boundary {
    boundary_len = p - boundary_start;
  }

  boundary = "--" [^ \t\r\n]+ >start_boundary %end_boundary;
  whitespace_boundary = "--" [^\r\n]* [ \t]+ >error_boundary_whitespace;
  quoted_boundary = '"' [^"]+ '"' >error_boundary_quoted;

  main := ' '* "multipart/form-data;" ' '* "boundary="
  (boundary | quoted_boundary | whitespace_boundary);
}%%

%% write data;

static std::string_view parseContentType(std::string_view input, SrSecurity::MultipartStrictError& error_code) {
  using namespace SrSecurity;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  int cs;

  const char* boundary_start = nullptr;
  size_t boundary_len = 0;

  %% write init;
  %% write exec;

  return { boundary_start, boundary_len };
}

%%{
  machine multipart;

  action skip {}

  action error_data_before {
    MULTI_PART_LOG("error_data_before");
    error_code.set(MultipartStrictError::ErrorType::DataBefore);
    fbreak;
  }

  action error_data_after {
    MULTI_PART_LOG("error_data_after");
    error_code.set(MultipartStrictError::ErrorType::DataAfter);
    fbreak;
  }

  action error_header_folding {
    MULTI_PART_LOG("error_header_folding");
    error_code.set(MultipartStrictError::ErrorType::HeaderFolding);
    fbreak;
  }

  action error_lf_line {
    MULTI_PART_LOG("error_lf_line");
    error_code.set(MultipartStrictError::ErrorType::LfLine);
    fbreak;
  }

  action error_missing_semicolon {
    MULTI_PART_LOG("error_missing_semicolon");
    error_code.set(MultipartStrictError::ErrorType::MissingSemicolon);
    fbreak;
  }

  action error_invalid_quoting {
    MULTI_PART_LOG("error_invalid_quoting");
    error_code.set(MultipartStrictError::ErrorType::InvalidQuoting);
    fbreak;
  }

  action error_invalid_part {
    MULTI_PART_LOG("error_invalid_part");
    error_code.set(MultipartStrictError::ErrorType::InvalidPart);
    fbreak;
  }

  action error_invalid_header_folding {
    MULTI_PART_LOG("error_invalid_header_folding");
    error_code.set(MultipartStrictError::ErrorType::InvalidHeaderFolding);
    fbreak;
  }

  action error_file_limit_exceeded {
    MULTI_PART_LOG("error_file_limit_exceeded");
    error_code.set(MultipartStrictError::ErrorType::FileLimitExceeded);
    fbreak;
  }

  action start_boundary {
    MULTI_PART_LOG("start_boundary");
    boundary_start = p - 2;
  }

  action end_boundary {
    boundary_len = p - boundary_start;
    MULTI_PART_LOG(std::format("end_boundary:{}", std::string_view(boundary_start, boundary_len)));
    if(boundary != std::string_view(boundary_start, boundary_len)) {
      MULTI_PART_LOG("error_unmatched_boundary");
      error_code.set(MultipartStrictError::ErrorType::UnmatchedBoundary);
      fbreak;
    }
  }

  boundary = "--" [^ \t\r\n\-]+ >start_boundary %end_boundary;
  data_before = [^\-] >error_data_before;
  data_after = any >error_data_after;

  lf = '\n' >error_lf_line;
  crlf = '\r\n';

  main := 
    ((data_before | boundary ) (("--" data_after?) %{ MULTI_PART_LOG("multipart end"); fbreak; })? (lf | crlf) @{ MULTI_PART_LOG("fcall headers"); fcall headers;})+;

  headers := |*
    # Header folding
    [ \t]+ => error_header_folding;

    # Invalid header folding
    [^ \t\r\n] [^ \t\r\n:]+ (lf | crlf) => error_invalid_header_folding;

    # Content-Disposition header
    "content-disposition:" [ \t]* => { 
      MULTI_PART_LOG("fcall content-disposition header_value");
      is_content_disposition = true;
      name = {};
      filename = {};
      p_value_start = nullptr;
      value_len = 0;
      fcall header_value; 
    };

    # Other headers
    [^ \t\r\n]+ ':' [ \t]* => { 
      MULTI_PART_LOG("fcall header_value");
      header_name = trim(ts, te - ts);
      header_name.remove_suffix(1);
      fcall header_value; 
    };

    # End of headers
    '\r\n' => { 
      MULTI_PART_LOG("fnext body"); 
      p_value_start = te;
      fnext body; 
    }; 

    any => error_invalid_part;
  *|;

  header_value := |*
    # Missing semicolon
    [^ \r\n;]+ [ \t]+ [^\r\n]+ => error_missing_semicolon;

    # The value is ok 
    [^\r\n]+ => { 
      if(is_content_disposition) {
        MULTI_PART_LOG("fnext content_disposition_value");
        p = ts; 
        fhold; 
        fnext content_disposition_value; 
      } else {
        // The value is the entire part-header line -- including both the part-header name and the part-header value.
        std::string_view header_value(header_name.data(), te - header_name.data());
        MULTI_PART_LOG(std::format("insert header key:{},value:{}",header_name, header_value));
        auto iter = headers_map.insert({ header_name, header_value});
        headers_linked.emplace_back(iter);
      }
    };

    # End of kv pair
    '\r\n' => {
      MULTI_PART_LOG("fret header_value");
      is_content_disposition = false;
      header_name = {};
      fret; 
    };

    any => error_invalid_part;
  *|;

  content_disposition_value := |*
    "form-data;" [ \t]* => skip;
    "name=" '"' [^"\r\n]+ '"' [ \t;]* => { 
      name = trim(ts + 5, te - ts - 5);
      MULTI_PART_LOG(std::format("name with quotes:{}", name));
    };
    "name=" [^ \t\r\n"]* '"' => error_invalid_quoting;
    "name=" [^ \t\r\n";]+ [ \t;]* => { 
      name = trim(ts + 5, te - ts - 5);
      MULTI_PART_LOG(std::format("name without quotes:{}", name));
      MULTI_PART_LOG(std::string_view(ts, te - ts));
    };
    "filename=" '"' [^"\r\n]+ '"' [ \t;]* => {
      filename = trim(ts + 9, te - ts - 9);
      MULTI_PART_LOG(std::format("filename with quotes:{}", filename));
      ++file_count; 
      if(max_file_count && file_count > max_file_count) { 
        error_code.set(MultipartStrictError::ErrorType::FileLimitExceeded); 
        fbreak; 
      }
    };
    "filename=" [^ \t\r\n"]* '"' => error_invalid_quoting;
    "filename=" [^ \t\r\n;]+ [ \t;]* => {
      filename = trim(ts + 9, te - ts - 9);
      MULTI_PART_LOG(std::format("filename without quotes:{}", filename));
      ++file_count; 
      if(max_file_count && file_count > max_file_count) { 
        error_code.set(MultipartStrictError::ErrorType::FileLimitExceeded); 
        fbreak; 
      }
    };
    '\r\n' => {
      if(name.empty()) {
        MULTI_PART_LOG("error_invalid_part: name is empty");
        error_code.set(MultipartStrictError::ErrorType::InvalidPart);
        fbreak;
      }

      MULTI_PART_LOG("fret content_disposition_value");
      is_content_disposition = false;
      header_name = {};
      fret; 
    };
    any => error_invalid_part;
  *|;

  body := |*
    "--" [^ \t\r\n\-]+ => { 
      if(boundary == std::string_view(ts, te - ts)) {
        if(filename.empty()) {
          if(value_len > 0) {
            MULTI_PART_LOG(std::format("add name:{}, value:{}", name, std::string_view(p_value_start, value_len)));
            auto result = name_value_map.insert({name, std::string_view(p_value_start, value_len)});
            name_value_linked.emplace_back(result);
          }
        }else{
          MULTI_PART_LOG(std::format("add name:{}, filename:{}", name, filename));
          auto result = name_filename_map.insert({name, filename});
          name_filename_linked.emplace_back(result);
        }

        parse_complete = true;

        MULTI_PART_LOG("body fret");
        p = ts;
        fhold;
        fret;
      }
    };
    any => { ++value_len; };
  *|;
}%%

%% write data;



// Trims leading and trailing whitespace or quotes from a string.
static std::string_view trim(const char* start, size_t size) {
  const char* end = start + size;

  // Trim leading whitespace or quotes
  while (start < end && (*start == ' ' || *start == '\t' || *start == '"')) {
    ++start;
  }

  // Trim trailing whitespace
  while (end > start && (*(end - 1) == ' ' || *(end - 1) == '\t' || *(end - 1) == '"' || *(end - 1) == ';')) {
    --end;
  }

  return std::string_view(start, end - start);
}

static void parseMultiPart(std::string_view input, 
  std::string_view boundary, 
  std::unordered_multimap<std::string_view, std::string_view>& name_value_map,
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>& name_value_linked, 
  std::unordered_multimap<std::string_view, std::string_view>& name_filename_map,
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>& name_filename_linked, 
  std::unordered_multimap<std::string_view, std::string_view>& headers_map,
  std::vector<std::unordered_multimap<std::string_view, std::string_view>::iterator>& headers_linked, 
  SrSecurity::MultipartStrictError& error_code, 
  uint32_t max_file_count) {
  using namespace SrSecurity;

  name_value_map.clear();
  name_value_linked.clear();
  name_filename_map.clear();
  name_filename_linked.clear();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;
  int top = 0;
  int stack[16];

  const char* boundary_start = nullptr;
  size_t boundary_len = 0;
  bool is_content_disposition = false;
  std::string_view name;
  std::string_view filename;
  const char* p_value_start = nullptr;
  size_t value_len = 0;
  uint32_t file_count = 0;
  bool parse_complete = false;
  std::string_view header_name;

  %% write init;
  %% write exec;

  if(!parse_complete && !error_code.get(MultipartStrictError::ErrorType::MultipartStrictError)) {
    error_code.set(MultipartStrictError::ErrorType::InvalidPart);
  }

  if(error_code.get(MultipartStrictError::ErrorType::MultipartStrictError)) {
    name_value_map.clear();
    name_value_linked.clear();
    name_filename_map.clear();
    name_filename_linked.clear();
  }
}

#undef MULTI_PART_LOG