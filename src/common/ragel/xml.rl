#pragma once

#include <string_view>
#include <vector>

#ifndef ENABLE_XML_DEBUG_LOG
#define ENABLE_XML_DEBUG_LOG 0
#endif

#if ENABLE_XML_DEBUG_LOG
#include <iostream>
#include <format>
#define XML_LOG(x) std::cout << x << std::endl;
#else
#define XML_LOG(x)
#endif

%%{
    machine xml;

    action skip {}

    action error {
      XML_LOG("error"); 
      error = true; 
      fbreak;
    }

    WS = [ \t\r\n]*;
    
    main := |*
      WS => skip;
      '<' => { XML_LOG("fcall open_tag"); fcall open_tag; };
      any => error;
    *|;

    open_tag := |*
      WS => skip;
      [^ =]+ '=' ['"] => { XML_LOG("fcall attr_value"); fcall attr_value; };
      '>' => { XML_LOG("fnext tag_value"); fnext tag_value; };
      '<' => error;
      any => skip;
    *|;

    attr_value := |*
      WS => skip;
      ['"] => { XML_LOG("fret attr_value"); fret; };
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
      WS => skip;
      '</' => { XML_LOG("fnext close_tag"); fnext close_tag; };
      '<' => { XML_LOG("fgoto open_tag"); fgoto open_tag; };
      [^<]+ => { XML_LOG(std::format("add tag value:{}",std::string_view(ts, te - ts))); tag_values.emplace_back(ts, te - ts); };
      any => error;
    *|;

    close_tag := |*
      WS => skip;
      [^>]+ => skip;
      '>' => { XML_LOG("fret close_tag"); fret; };
      any => error;
    *|;
}%%

%% write data;

static bool parseXml(std::string_view input, std::vector<std::string_view>& attr_values,
  std::vector<std::string_view>& tag_values) {
  attr_values.clear();
  tag_values.clear();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;
  int top = 0;
  int stack[16];

  bool error = false;

  %% write init;
  %% write exec;

  return error;
}

#undef XML_LOG