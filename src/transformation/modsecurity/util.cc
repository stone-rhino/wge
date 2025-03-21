/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2025 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * Modifications made by Stone Rhino, 2025.
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 */
#include "util.h"

namespace SrSecurity::Transformation::ModSecurity {
std::string cmdLime(std::string_view data) {
  // The cmdLine transformation function avoids this problem by manipulating the variable contend
  // in the following ways:
  // - deleting all backslashes [\]
  // - deleting all double quotes ["]
  // - deleting all single quotes [']
  // - deleting all carets [^]
  // - deleting spaces before a slash /
  // - deleting spaces before an open parentesis [(]
  // - replacing all commas [,] and semicolon [;] into a space
  // - replacing all multiple spaces (including tab, newline, etc.) into one space
  // - transform all characters to lowercase
  std::string result;
  result.resize(data.length());
  char* d = result.data();

  bool space = false;
  for (auto ch : data) {
    switch (ch) {
    // Remove some characters
    case '"':
    case '\'':
    case '\\':
    case '^':
      break;
    // Replace some characters to space (only one)
    case ' ':
    case ',':
    case ';':
    case '\t':
    case '\r':
    case '\n':
      if (space == false) {
        *d++ = ' ';
        space = true;
      }
      break;
    // Remove space before / or (
    case '/':
    case '(':
      if (space) {
        d--;
      }
      space = false;
      *d++ = ch;
      break;
    // Copy normal characters
    default:
      char b = std::tolower(ch);
      *d++ = b;
      space = false;
      break;
    }
  }

  const auto new_len = d - result.data();
  const auto changed = new_len != data.length();

  if (changed) {
    result.resize(new_len);
  }

  return result;
}
} // namespace SrSecurity::Transformation::ModSecurity