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
  action tolower { *r++ = tolower(fc); }

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

static bool cmdLine(std::string_view input, std::string& result) {
  result.clear();
  char* r = nullptr;

  const char* p = input.data();
  const char* pe = p + input.size();
  const char* eof = pe;
  const char* ts, *te;
  int cs,act;

  %% write init;
  %% write exec;

  if(r) {
    result.resize(r - result.data());
    return true;
  }

  return false;
}