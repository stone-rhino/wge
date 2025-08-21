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

    action open_tag {
      tags_name_index.push(tags.size());
      tags.emplace_back(std::string_view(ts + 1, te - ts -1),"");
      XML_LOG(std::format("fgoto open_tag: {} current open tags count: {}",std::string_view(ts + 1, te - ts -1), tags_name_index.size()));
      p = ts + 1;
      fhold;
      fgoto open_tag;
    }

    WS = [ \t\r\n]*;
    
    main := |*
      WS => { tag_values_str.append(ts, te - ts); };
      '<?' ([^?] | ('?' [^>]))* '?>' WS => { XML_LOG(std::format("skip processing instruction: {}",std::string_view(ts, te - ts))); };
      '<!--' ([^\-] | ('-' [^\-]))*  '-->' WS => { XML_LOG(std::format("skip comment: {}",std::string_view(ts, te - ts))); };
      # skip doctype without internal subset
      '<!DOCTYPE' [^>]+ '>' WS => { XML_LOG(std::format("skip doctype: {}",std::string_view(ts, te - ts))); };
      # skip doctype with internal subset
      '<!DOCTYPE' [^>]+ '[' [^\]]+ ']'  '>' WS => { XML_LOG(std::format("skip doctype with internal subset: {}",std::string_view(ts, te - ts))); };
      '<' [^ !/\t>]+ => open_tag;
      '</' [^>]+ '>' => skip; 
      any => error;
    *|;

    open_tag := |*
      WS => skip;
      [^ =]+ '=' ['"] => {
        last_attr_name = std::string_view(ts, te - ts - 2);
        XML_LOG("fgoto attr_value");
        fgoto attr_value;
      };
      '/>' => {
        tags_name_index.pop();

        XML_LOG("fgoto main");
        fgoto main;
      };
      '>' => { XML_LOG("fgoto tag_value"); fgoto tag_value; };
      '<' => error;
      any => skip;
    *|;

    attr_value := |*
      WS => skip;
      ['"] => { XML_LOG("fgoto open_tag"); fgoto open_tag; };
      [^'" \t\r\n]+ => { 
        if(te == pe) {
          error = true;
          fbreak;
        }
        XML_LOG(std::format("add attr value: {}={}", last_attr_name, std::string_view(ts, te - ts)));
        attributes.emplace_back(last_attr_name, std::string_view(ts, te - ts));
      };
    *|;

    tag_value := |*
      WS => { tag_values_str.append(ts, te - ts); };
      '</' [^>]+ '>' => {
        tags_name_index.pop();

        if(tags_name_index.empty()) {
          XML_LOG("fgoto main");
          fgoto main;
        }
      };
      '<![CDATA[' => { XML_LOG("fgoto tag_cdata_value"); fgoto tag_cdata_value; };
      '<?' ([^?] | ('?' [^>]))* '?>' => { XML_LOG(std::format("skip processing instruction: {}",std::string_view(ts, te - ts))); };
      '<!--' ([^\-] | ('-' [^\-]))*  '-->' => { XML_LOG(std::format("skip comment: {}",std::string_view(ts, te - ts))); };
      # skip doctype without internal subset
      '<!DOCTYPE' [^>]+ '>' => { XML_LOG(std::format("skip doctype: {}",std::string_view(ts, te - ts))); };
      # skip doctype with internal subset
      '<!DOCTYPE' [^>]+ '[' [^\]]+ ']'  '>' => { XML_LOG(std::format("skip doctype with internal subset: {}",std::string_view(ts, te - ts))); };
      '<' [^ !/\t>]+ => open_tag;
      [^<] => {
        XML_LOG("fgoto tag_string_value");
        p = ts;
        fhold;
        fgoto tag_string_value;
      };
      any => error;
    *|;

    tag_string_value := |*
      WS => { tag_values_str.append(ts, te - ts); };
      '</' [^>]+ '>' => {
        tags_name_index.pop();
        if(tags_name_index.empty()) {
          XML_LOG("fgoto main");
          fgoto main;
        } else {
          XML_LOG("fgoto tag_value");
          fgoto tag_value;
        }
      };
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
      };
      '<?' ([^?] | ('?' [^>]))* '?>' => { XML_LOG(std::format("skip processing instruction: {}",std::string_view(ts, te - ts))); };
      '<!--' ([^\-] | ('-' [^\-]))*  '-->' => { XML_LOG(std::format("skip comment: {}",std::string_view(ts, te - ts))); };
      # skip doctype without internal subset
      '<!DOCTYPE' [^>]+ '>' => { XML_LOG(std::format("skip doctype: {}",std::string_view(ts, te - ts))); };
      # skip doctype with internal subset
      '<!DOCTYPE' [^>]+ '[' [^\]]+ ']'  '>' => { XML_LOG(std::format("skip doctype with internal subset: {}",std::string_view(ts, te - ts))); };
      '<' [^ !/\t>]+ => open_tag;
      any => error;
    *|;

    tag_cdata_value := |*
      WS => { tag_values_str.append(ts, te - ts); };
      ']]>' WS '</' [^>]+ '>' => {
        tags_name_index.pop();
        if(tags_name_index.empty()) {
          XML_LOG("fgoto main");
          fgoto main;
        } else {
          XML_LOG("fgoto tag_value");
          fgoto tag_value;
        }
      };
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
      any => error; 
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

        // clang-format off
	%% write init;
  %% write exec;
        // clang-format on

        return error;
      }

#undef XML_LOG