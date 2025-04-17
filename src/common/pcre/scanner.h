#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <stdint.h>

#include "pattern.h"

namespace SrSecurity {
namespace Common {
namespace Pcre {
class Scanner {
public:
  Scanner(const std::string& pattern, bool case_less, bool captrue);
  Scanner(std::string_view pattern, bool case_less, bool captrue);
  Scanner(const PatternList* pattern_list);
  ~Scanner();

public:
  const Pattern* getPattern(uint64_t id);
  void match(std::string_view subject, std::vector<std::pair<size_t, size_t>>& result) const;
  void match(uint64_t id, std::string_view subject,
             std::vector<std::pair<size_t, size_t>>& result) const;
  void match(const Pattern* pattern, std::string_view subject,
             std::vector<std::pair<size_t, size_t>>& result) const;
  bool match(const Pattern* pattern, std::string_view subject) const;
  bool match(std::string_view subject) const;
  void matchGlobal(std::string_view subject, std::vector<std::pair<size_t, size_t>>& result) const;
  void matchGlobal(uint64_t id, std::string_view subject,
                   std::vector<std::pair<size_t, size_t>>& result) const;
  void matchGlobal(const Pattern* pattern, std::string_view subject,
                   std::vector<std::pair<size_t, size_t>>& result) const;
  void setMatchLimit(size_t match_limit);

private:
  std::unique_ptr<Pattern> pattern_;
  void* match_context_{nullptr};
  const PatternList* pattern_list_{nullptr};
  static thread_local Scratch per_thread_scratch_;
};
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity