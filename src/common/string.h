#pragma once

#include <string_view>
#include <vector>

namespace SrSecurity {
namespace Common {
static std::vector<std::string_view> SplitTokens(std::string_view value, char delimiter = ' ') {
  std::vector<std::string_view> tokens;
  size_t pos = 0;
  size_t next_pos = 0;

  while ((next_pos = value.find(delimiter, pos)) != std::string_view::npos) {
    tokens.emplace_back(value.substr(pos, next_pos - pos));
    pos = next_pos + 1;
  }

  if (pos < value.size()) {
    tokens.emplace_back(value.substr(pos));
  }

  return tokens;
}
} // namespace Common
} // namespace SrSecurity
