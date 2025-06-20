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
#include <string_view>
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
      XML_LOG(std::format("fgoto open_tag: {}",std::string_view(ts + 1, te - ts -1)));
      last_tag_name = std::string_view(ts + 1, te - ts -1);
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
      [^ =]+ '=' ['"] => { XML_LOG("fgoto attr_value"); fgoto attr_value; };
      '/>' => {
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
        XML_LOG(std::format("add attr value:{}",std::string_view(ts, te - ts)));
        attr_values.emplace_back(ts, te - ts);
      };
    *|;

    tag_value := |*
      WS => { tag_values_str.append(ts, te - ts); };
      '</' [^>]+ => {
        std::string_view tag_name(ts + 2, te - ts - 2);
        XML_LOG(std::format("find close_tag: {}", tag_name));
        if(tag_name == last_tag_name) {
          XML_LOG("fgoto close_tag");
          fgoto close_tag;
        }
      };
      '<![CDATA[' => { XML_LOG("fgoto tag_cdata_value"); fgoto tag_cdata_value; };
      '<' [^ !/\t>]+ => open_tag;
      [^<] => {
        tag_value_start = ts;
        XML_LOG("fgoto tag_value_no_open_tag");
        p = ts;
        fhold;
        fgoto tag_value_no_open_tag;
      };
      any => error;
    *|;

    tag_value_no_open_tag := |*
      '</' [^>]+ => {
        std::string_view tag_name(ts + 2, te - ts - 2);
        XML_LOG(std::format("find close_tag: {}", tag_name));
        if(tag_name == last_tag_name) {
          if(tag_value_start) {
            std::string_view tag_value(tag_value_start, tag_value_len);
            std::string buffer;
            bool success = htmlEntityDecode(tag_value, buffer);
            if(success) {
              html_decode_buffer.emplace_front(std::move(buffer));
              tag_value = html_decode_buffer.front();
            }
            XML_LOG(std::format("add tag value:{}", tag_value));
            tag_values.emplace_back(tag_value);
            tag_values_str.append(tag_value);
            tag_value_start = nullptr;
            tag_value_len = 0;
          }

          XML_LOG("fgoto close_tag");
          fgoto close_tag;
        } else {
          tag_value_len += te - ts;
        }
      };
      any => { ++tag_value_len; };
    *|;

    tag_cdata_value := |*
      WS => skip;
      ']]>' => { XML_LOG("fgoto tag_value"); fgoto tag_value; };
      [^\]]+ => { XML_LOG(std::format("add tag cdata value:{}",std::string_view(ts, te - ts))); tag_values.emplace_back(ts, te - ts); tag_values_str.append(ts, te - ts); };
      any => error; 
    *|;

    close_tag := |*
      WS => skip;
      [^>]+ => skip;
      '>' => { XML_LOG("fgoto main"); fgoto main; };
      any => error;
    *|;
}%%

%% write data;
      // clang-format on

      static bool parseXml(std::string_view input, std::vector<std::string_view>& attr_values,
                           std::vector<std::string_view>& tag_values, std::string& tag_values_str,
                           std::forward_list<std::string> html_decode_buffer) {

        const char* p = input.data();
        const char* pe = p + input.size();
        const char* eof = pe;
        const char *ts, *te;
        int cs, act;
        int top = 0;
        int stack[16];

        bool error = false;
        std::string_view last_tag_name;
        const char* tag_value_start = nullptr;
        size_t tag_value_len = 0;

        // clang-format off
	%% write init;
  %% write exec;
        // clang-format on

        return error;
      }

#undef XML_LOG