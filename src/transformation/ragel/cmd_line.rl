#pragma once

#include <string>
#include <string_view>

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

  main := |*
    '\\' => {};
    '"' => {};
    '\'' => {};
    '^' => {};
    [ \t\r\n,;]+'/' => { *r++ = '/'; };
    [ \t\r\n,;]+'(' => { *r++ = '('; };
    [ \t\r\n,;]+ => { *r++ = ' '; };
    any => { *r++ = tolower(*p); };
  *|;
}%%

%% write data;

std::string cmdLine(std::string_view input) {
  std::string result;
  result.resize(input.size());
  char* r = result.data();

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  result.resize(r - result.data());
  return result;
}