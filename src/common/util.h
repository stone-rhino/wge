#pragma once

#include <string>

namespace SrSecurity {
class Util {
public:
  static std::string regexEscape(const std::string& str) {
    std::string escape;

    const char* p = str.data();
    const char* end = p + str.length();
    while (p != end) {
      switch (*p) {
      case '^':
      case '$':
      case '{':
      case '}':
      case '[':
      case ']':
      case '(':
      case ')':
      case '|':
      case '.':
      case '*':
      case '+':
      case '?':
        escape += '\\';
        escape += *p;
        break;
      case '\\':
        if (p + 1 < end) {
          escape += *p;
          escape += *(p + 1);
        }
        ++p;
        break;
      default:
        escape += *p;
        break;
      }

      ++p;
    }

    return escape;
  }
};
} // namespace SrSecurity