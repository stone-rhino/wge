#pragma once

#include <string>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class CmdLine : public TransformBase {
  DECLARE_TRANSFORM_NAME(cmdLine);

public:
  std::string evaluate(std::string_view data) const override {
    // The cmdLine transformation function avoids this problem by manipulating the variable contend
    // in the following ways:
    // deleting all backslashes [\]
    // deleting all double quotes ["]
    // deleting all single quotes [']
    // deleting all carets [^]
    // deleting spaces before a slash /
    // deleting spaces before an open parentesis [(]
    // replacing all commas [,] and semicolon [;] into a space
    std::string result;
    for (size_t i = 0; i < data.length(); ++i) {
      switch (data[i]) {
      case '\\':
      case '"':
      case '\'':
      case '^':
        break;
      case ' ':
        if (i + 1 < data.length() && (data[i + 1] == '/' || data[i + 1] == '(')) {
          break;
        }
        result.push_back(data[i]);
        break;
      case ',':
      case ';':
        result.push_back(' ');
        break;
      default:
        result.push_back(data[i]);
        break;
      }
    }

    // replacing all multiple spaces (including tab, newline, etc.) into one space
    // transform all characters to lowercase
    bool is_space = false;
    for (size_t i = 0; i < data.length(); ++i) {
      if (std::isspace(data[i])) {
        if (!is_space) {
          result.push_back(' ');
          is_space = true;
        }
      } else {
        result.push_back(std::tolower(data[i]));
        is_space = false;
      }
    }

    return result;
  }
};
} // namespace Transformation
} // namespace SrSecurity
