/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

#include <forward_list>
#include <stack>
#include <string_view>
#include <utility>
#include <vector>

#include <html_entity_decode.h>

#ifndef ENABLE_XML_DEBUG_LOG
#define ENABLE_XML_DEBUG_LOG 0
#endif

#if ENABLE_XML_DEBUG_LOG
#include <format>
#include <iostream>
#define XML_LOG(x) std::cout << x << std::endl;
#else
#define XML_LOG(x)
#endif

// clang-format off
%%{
    machine xml;

    action skip {}

    action error {
      XML_LOG("error"); 
      error = true; 
      fbreak;
    }

    WS = [ \t\r\n];
    NAME = [a-zA-Z_:][a-zA-Z0-9\-_:.]*;
    
    main := |*
      WS+ => {
        // Ingnore whitespace before root tag
        if(!tags_name_index.empty()) {
          tag_values_str.append(ts, te - ts);
        }
      };
      '<' => { XML_LOG("main fgoto open_tag"); fgoto open_tag; };
      any => error;
    *|;

    open_tag := |*
      '?' => { XML_LOG("open_tag fgoto instruction"); fgoto instruction; };
      '!' => { XML_LOG("open_tag fgoto comment_or_doctype"); fgoto comment_or_doctype; };
      '/' NAME* '>' => {
        tags_name_index.pop();
        if(tags_name_index.empty()){
          XML_LOG("open_tag fgoto main");
          fgoto main;
        } else {
          XML_LOG("open_tag fgoto tag_value");
          fgoto tag_value;
        }
      };
      NAME => {
        tags_name_index.push(tags.size());
        tags.emplace_back(std::string_view(ts, te),"");
        XML_LOG(std::format("tag_name: {}, current tags count: {}",std::string_view(ts, te), tags_name_index.size()));
        XML_LOG("open_tag fgoto attr");
        fgoto attr;
      };
      any => error;
    *|;

    instruction := |*
      '?>' => { XML_LOG("instruction fgoto main"); fgoto main; };
      any => skip;
    *|;

    comment_or_doctype := |*
      'DOCTYPE' => { XML_LOG("comment_or_doctype fgoto doctype"); fgoto doctype; };
      '--' => { XML_LOG("comment_or_doctype fgoto comment"); fgoto comment; };
      '[CDATA[' => { XML_LOG("comment_or_doctype fgoto cdata"); fgoto cdata; };
      any => error;
    *|;

    doctype := |*
      '>' => { XML_LOG("doctype fgoto main"); fgoto main; };
      '[' => { XML_LOG("doctype fgoto doctype_internal_subset"); fgoto doctype_internal_subset; };
      any => skip;
    *|;

    doctype_internal_subset := |*
      ']' => { XML_LOG("doctype_internal_subset fgoto doctype"); fgoto doctype; };
      any => skip;
    *|;

    comment := |*
      '-->' => { XML_LOG("comment fgoto main"); fgoto main; };
      any => skip;
    *|;

    cdata := |*
      ']]>' => { XML_LOG("cdata fgoto tag_value"); fgoto tag_value; };
      [^\]]+ => {
        size_t index = tags_name_index.top();
        if(!tags.empty() && tags.size() > index) {
          auto& [tag_name, tag_value] = tags[index];
          if(tag_value.empty()) {
            tag_value = std::string_view(ts, te - ts);
            XML_LOG(std::format("set tag cdata value[{}] {}: {}", index, tag_name, std::string_view(ts, te - ts)));
          } else {
            // If there is multiple text nodes, concatenate them
            std::string concatenated_value;
            concatenated_value = tag_value;
            concatenated_value += std::string_view(ts, te - ts);
            html_decode_buffer.emplace_front(std::move(concatenated_value));
            tag_value = html_decode_buffer.front();
            XML_LOG(std::format("concat tag cdata value[{}] {}: {}", index, tag_name, std::string_view(ts, te - ts)));
            XML_LOG(std::format("final {}: {}", tag_name, tag_value));
          }
        }

        tag_values_str.append(ts, te - ts);
      };
    *|;

    attr := |*
      WS => skip;
      NAME '=' ['"] => {
        last_attr_name = std::string_view(ts, te - ts - 2);
        XML_LOG("attr fgoto attr_value");
        fgoto attr_value;
      };
      '/>' => {
        tags_name_index.pop();

        XML_LOG("attr fgoto main");
        fgoto main;
      };
      '>' => { XML_LOG("attr fgoto tag_value"); fgoto tag_value; };
      any => error;
    *|;

    attr_value := |*
      ['"] => { XML_LOG("attr_value fgoto attr"); fgoto attr; };
      [^'"]+ => { 
        XML_LOG(std::format("add attr value: {}={}", last_attr_name, std::string_view(ts, te - ts)));
        attributes.emplace_back(last_attr_name, std::string_view(ts, te - ts));
      };
    *|;

    tag_value := |*
      WS+ => { tag_values_str.append(ts, te - ts); };
      [^<] => {
        XML_LOG("tag_value fgoto tag_string_value");
        p = ts;
        fhold;
        fgoto tag_string_value;
      };
      '<' => { XML_LOG("tag_value fgoto open_tag"); fgoto open_tag; };
    *|;

    tag_string_value := |*
      [^<]+ => {
        std::string_view tag_value_view(ts, te);
        std::string buffer;
        bool success = htmlEntityDecode(tag_value_view, buffer);
        if(success) {
          html_decode_buffer.emplace_front(std::move(buffer));
          tag_value_view = html_decode_buffer.front();
        }

        size_t index = tags_name_index.top();
        if(!tags.empty() && tags.size() > index) {
          auto& [tag_name, tag_value] = tags[index];
          if(tag_value.empty()) {
            tag_value = tag_value_view;
            XML_LOG(std::format("set tag value[{}] {}: {}", index, tag_name, tag_value_view));
          } else {
            // If there is multiple text nodes, concatenate them
            std::string concatenated_value;
            concatenated_value = tag_value;
            concatenated_value += tag_value_view;
            html_decode_buffer.emplace_front(std::move(concatenated_value));
            tag_value = html_decode_buffer.front();
            XML_LOG(std::format("concat tag value[{}] {}: {}", index, tag_name, tag_value_view));
            XML_LOG(std::format("final {}: {}", tag_name, tag_value));
          }

          tag_values_str.append(tag_value_view);
        }

        XML_LOG("tag_string_value fgoto tag_value");
        fgoto tag_value;
      };
    *|;
}%%

%% write data;
    // clang-format on

    static bool parseXml(std::string_view input,
                         std::vector<std::pair<std::string_view, std::string_view>>& attributes,
                         std::vector<std::pair<std::string_view, std::string_view>>& tags,
                         std::string& tag_values_str,
                         std::forward_list<std::string>& html_decode_buffer) {

      const char* p = input.data();
      const char* pe = p + input.size();
      const char* eof = pe;
      const char *ts, *te;
      int cs, act;
      int top = 0;

      bool error = false;
      std::stack<size_t> tags_name_index;
      std::string_view last_attr_name;

      // Ensure all tags are closed
      if (!tags_name_index.empty()) {
        error = true;
      }

      // clang-format off
	%% write init;
  %% write exec;
      // clang-format on

      return error;
    }

#undef XML_LOG