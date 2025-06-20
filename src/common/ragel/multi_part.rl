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

#include <algorithm>
#include <string_view>

#ifndef ENABLE_MULTI_PART_DEBUG_LOG
#define ENABLE_MULTI_PART_DEBUG_LOG 0
#endif

#if ENABLE_MULTI_PART_DEBUG_LOG
#include <format>
#include <iostream>
#define MULTI_PART_LOG(x) std::cout << x << std::endl;
#else
#define MULTI_PART_LOG(x)
#endif

// clang-format off
%%{
  machine multipart_content_type;
  
  action error_boundary_quoted {
    MULTI_PART_LOG("error_boundary_quoted");
    error_code.set(MultipartStrictError::ErrorType::BoundaryQuoted);
    boundary_start = p;
  }

  action start_boundary_name {
    boundary_start = p;
  }

  action boundary_name {
    switch(fc){
      case ' ': 
      case '\t': {
        MULTI_PART_LOG("error_boundary_whitespace");
        error_code.set(MultipartStrictError::ErrorType::BoundaryWhitespace);
        break;
      }
      case '"': {
        MULTI_PART_LOG("error_boundary_quoted");
        error_code.set(MultipartStrictError::ErrorType::BoundaryQuoted);
        break;
      }
      default: 
        break;
    }
  }

  action end_boundary_name {
    boundary_len = p - boundary_start;
    MULTI_PART_LOG(std::format("end_boundary_name:{}", std::string_view(boundary_start, boundary_len)));
  }

  boundary = [^\r\n]+ >start_boundary_name @boundary_name %end_boundary_name;

  main := ' '* "multipart/form-data;" ' '* "boundary=" boundary;
}%%

%% write data;
// clang-format on

static std::string_view parseContentType(std::string_view input,
                                         Wge::MultipartStrictError& error_code) {
  using namespace Wge;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  int cs;

  const char* boundary_start = nullptr;
  size_t boundary_len = 0;

  // clang-format off
	%% write init;
  %% write exec;
  // clang-format on

  return {boundary_start, boundary_len};
}

// clang-format off
%%{
  machine multipart;

  action skip {}

  action error_data_before {
    MULTI_PART_LOG("error_data_before");
    error_code.set(MultipartStrictError::ErrorType::DataBefore);
  }

  action error_data_after {
    MULTI_PART_LOG("error_data_after");
    error_code.set(MultipartStrictError::ErrorType::DataAfter);
    fbreak;
  }

  action error_header_folding {
    MULTI_PART_LOG("error_header_folding");
    error_code.set(MultipartStrictError::ErrorType::HeaderFolding);
  }

  action error_lf_line {
    // Disabled the lf line error check for now
    // MULTI_PART_LOG("error_lf_line");
    // error_code.set(MultipartStrictError::ErrorType::LfLine);
    // fbreak;
  }

  action error_missing_semicolon {
    MULTI_PART_LOG("error_missing_semicolon");
    error_code.set(MultipartStrictError::ErrorType::MissingSemicolon);
  }

  action error_invalid_quoting {
    MULTI_PART_LOG("error_invalid_quoting");
    error_code.set(MultipartStrictError::ErrorType::InvalidQuoting);
  }

  action error_invalid_part {
    MULTI_PART_LOG("error_invalid_part");
    error_code.set(MultipartStrictError::ErrorType::InvalidPart);
    fbreak;
  }

  action error_invalid_header_folding {
    MULTI_PART_LOG("error_invalid_header_folding");
    error_code.set(MultipartStrictError::ErrorType::InvalidHeaderFolding);
  }

  action error_file_limit_exceeded {
    MULTI_PART_LOG("error_file_limit_exceeded");
    error_code.set(MultipartStrictError::ErrorType::FileLimitExceeded);
  }

  action start_boundary {
    MULTI_PART_LOG("start_boundary");
    boundary_start = p;
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

  boundary = '--' [^\r\n]+ >start_boundary %end_boundary;
  data_before = [^\-] >error_data_before;

  lf = '\n' >error_lf_line;
  crlf = '\r\n';

  main := 
    (data_before* boundary (lf | crlf) @{ MULTI_PART_LOG("fcall headers"); fcall headers;})+;

  headers := |*
    # Invalid Part
    '--' [^\r\n]+ => {
      MULTI_PART_LOG("error_invalid_part: no body");
      error_code.set(MultipartStrictError::ErrorType::InvalidPart);
      p = ts;
      fhold;
      fret;
    };

    # Header folding
    [ \t]+ => error_header_folding;

    # Invalid header folding
    [^ \t\r\n\-] [^ \t\r\n:]+ (lf | crlf) => error_invalid_header_folding;

    [^ \t\r\n]+ ':' [ \t]* => { 
      MULTI_PART_LOG("fcall header_value");
      header_name = trim(ts, te - ts);
      header_name.remove_suffix(1);
      std::string lower_header_name;
      lower_header_name.reserve(header_name.size());
      std::transform(header_name.begin(), header_name.end(), std::back_inserter(lower_header_name), ::tolower);
      if(lower_header_name == "content-disposition") {
        is_content_disposition = true;
      } else {
        is_content_disposition = false;
      }

      fcall header_value; 
    };

    # End of headers
    (lf | crlf) => { 
      MULTI_PART_LOG("fnext body"); 
      p_value_start = te;
      fnext body; 
    }; 

    any => error_invalid_part;
  *|;

  header_value := |*
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
        auto iter = headers_map.insert({header_name, header_value});
        headers_linked.emplace_back(header_name, header_value);
      }
    };

    # End of kv pair
    (lf | crlf) => {
      MULTI_PART_LOG("fret header_value");
      is_content_disposition = false;
      header_name = {};
      fret; 
    };

    any => error_invalid_part;
  *|;

  content_disposition_value := |*
    "form-data;" [ \t]* => skip;
    "form-data" [ \t]* => error_missing_semicolon;
    "name=" '"' [^"\r\n]+ '"' [ \t;]* => { 
      name = trim(ts + 5, te - ts - 5);
      MULTI_PART_LOG(std::format("name with quotes:{}", name));
    };
    "name=" [^ \t\r\n"]* '"' [^ \t\r\n"]* => {
      MULTI_PART_LOG("error_invalid_quoting");
      error_code.set(MultipartStrictError::ErrorType::InvalidQuoting);
      name = trim(ts + 5, te - ts - 5);
      MULTI_PART_LOG(std::format("name:{}", name));
    };
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
      }
    };
    "filename=" [^ \t\r\n"]* '"' => error_invalid_quoting;
    "filename=" [^ \t\r\n;]+ [ \t;]* => {
      filename = trim(ts + 9, te - ts - 9);
      MULTI_PART_LOG(std::format("filename without quotes:{}", filename));
      ++file_count; 
      if(max_file_count && file_count > max_file_count) { 
        error_code.set(MultipartStrictError::ErrorType::FileLimitExceeded); 
      }
    };
    (lf | crlf) => {
      if(name.empty()) {
        MULTI_PART_LOG("error_invalid_part: name is empty");
        error_code.set(MultipartStrictError::ErrorType::InvalidPart);
      }

      MULTI_PART_LOG("fret content_disposition_value");
      is_content_disposition = false;
      header_name = {};
      fret; 
    };
    any => error_invalid_part;
  *|;

  body := |*
    "--" [^\r\n]+ => {
      std::string_view token = std::string_view(ts + 2, te - ts - 2);
      if(token.starts_with(boundary)) {
        if(filename.empty()) {
          if(value_len > 0) {
            MULTI_PART_LOG(std::format("add name:{}, value:{}", name, std::string_view(p_value_start, value_len)));
            auto result = name_value_map.insert({name, std::string_view(p_value_start, value_len)});
            name_value_linked.emplace_back(name, std::string_view(p_value_start, value_len));
          }
        }else{
          MULTI_PART_LOG(std::format("add name:{}, filename:{}", name, filename));
          auto result = name_filename_map.insert({name, filename});
          name_filename_linked.emplace_back(name, filename);
        }

        name = {};
        filename = {};
        p_value_start = nullptr;
        value_len = 0;

        if(token.size() == boundary.size()){
          MULTI_PART_LOG("body fret");
          p = ts;
          fhold;
          fret;
        } else if (token.size() == boundary.size() + 2 && token.ends_with("--")) {
          if(te != eof) {
            MULTI_PART_LOG("error_data_after");
            error_code.set(MultipartStrictError::ErrorType::DataAfter);
            fbreak;
          }

          MULTI_PART_LOG("multipart end"); 
          parse_complete = true;
        } else{
          MULTI_PART_LOG("error_data_after");
          error_code.set(MultipartStrictError::ErrorType::DataAfter);
          fbreak;
        }
      }
    };
    any => { ++value_len; };
  *|;
}%%

%% write data;
    // clang-format on

    // Trims leading and trailing whitespace or quotes from a string.
    static std::string_view trim(const char* start, size_t size) {
      const char* end = start + size;

      // Trim leading whitespace or quotes
      while (start < end && (*start == ' ' || *start == '\t' || *start == '"')) {
        ++start;
      }

      // Trim trailing whitespace
      while (end > start &&
             (*(end - 1) == ' ' || *(end - 1) == '\t' || *(end - 1) == '"' || *(end - 1) == ';')) {
        --end;
      }

      return std::string_view(start, end - start);
    }

    static void
    parseMultiPart(std::string_view input, std::string_view boundary,
                   std::unordered_multimap<std::string_view, std::string_view>& name_value_map,
                   std::vector<std::pair<std::string_view, std::string_view>>& name_value_linked,
                   std::unordered_multimap<std::string_view, std::string_view>& name_filename_map,
                   std::vector<std::pair<std::string_view, std::string_view>>& name_filename_linked,
                   std::unordered_multimap<std::string_view, std::string_view>& headers_map,
                   std::vector<std::pair<std::string_view, std::string_view>>& headers_linked,
                   Wge::MultipartStrictError& error_code, uint32_t max_file_count) {
      using namespace Wge;

      name_value_map.clear();
      name_value_linked.clear();
      name_filename_map.clear();
      name_filename_linked.clear();

      const char* p = input.data();
      const char* pe = p + input.size();
      const char* eof = pe;
      const char *ts, *te;
      int cs, act;
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

      // clang-format off
	%% write init;
  %% write exec;
      // clang-format on

      // If there is no end boundary, process the last part
      if (filename.empty()) {
        if (p_value_start && value_len > 0) {
          MULTI_PART_LOG("no end boundary, process last part");
          MULTI_PART_LOG(std::format("add name:{}, value:{}", name,
                                     std::string_view(p_value_start, value_len)));
          auto result = name_value_map.insert({name, std::string_view(p_value_start, value_len)});
          name_value_linked.emplace_back(name, std::string_view(p_value_start, value_len));
        }
      } else {
        MULTI_PART_LOG("no end boundary, process last part");
        MULTI_PART_LOG(std::format("add name:{}, filename:{}", name, filename));
        auto result = name_filename_map.insert({name, filename});
        name_filename_linked.emplace_back(name, filename);
      }

      if (!parse_complete &&
          !error_code.get(MultipartStrictError::ErrorType::MultipartStrictError)) {
        MULTI_PART_LOG("error_invalid_part: parse not complete");
        error_code.set(MultipartStrictError::ErrorType::InvalidPart);
      }

      // Does not need to clear the parse result when error occurs, keep the parse result as much as
      // possible if(error_code.get(MultipartStrictError::ErrorType::MultipartStrictError)) {
      //   name_value_map.clear();
      //   name_value_linked.clear();
      //   name_filename_map.clear();
      //   name_filename_linked.clear();
      // }
    }

#undef MULTI_PART_LOG