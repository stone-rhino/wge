#pragma once

#include <string>
#include <string_view>
#include <iostream>

// deleting all backslashes [\]
// deleting all double quotes ["]
// deleting all single quotes [']
// deleting all carets [^]
// deleting spaces before a slash /
// deleting spaces before an open parentesis [(]
// replacing all commas [,] and semicolon [;] into a space
// replacing all multiple spaces (including tab, newline, etc.) into one space
// transform all characters to lowercase
%%{
  machine cmd_line;

  action exec_transformation { 
    result.resize(input.size());
    r = result.data();
    if(ts > input.data()){
      memcpy(r, input.data(), ts - input.data());
      r += ts - input.data();
    }
    p = ts;
    fhold;
    fgoto transformation;
  }

  action skip {}
  action append_slash { *r++ = '/'; }
  action append_open_parenthesis { *r++ = '('; }
  action append_space { *r++ = ' '; }
  action tolower { *r++ = tolower(*p); }

  # prescan
  main := |*
    [\\"'^] => exec_transformation;
    [ \t\r\n,;]+'/' => exec_transformation;
    [ \t\r\n,;]+'(' => exec_transformation;
    [\t\r\n,;] => exec_transformation;
    ' '{2,} => exec_transformation;
    [A-Z] => exec_transformation;
    any => {};
  *|;

  transformation := |*
    [\\"'^] => skip;
    [ \t\r\n,;]+'/' => append_slash;
    [ \t\r\n,;]+'(' => append_open_parenthesis;
    [ \t\r\n,;]+ => append_space;
    any => tolower;
  *|;
}%%

%% write data;

std::string cmdLine(std::string_view input) {
  std::string result;
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r){
    result.resize(r - result.data());
  }
  return result;
}