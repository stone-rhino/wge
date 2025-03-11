#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <stdint.h>

#include "scratch.h"

namespace SrSecurity {
namespace Common {
namespace Pcre {
class Pattern {
public:
  Pattern(const std::string& pattern, bool case_less);
  Pattern(std::string_view pattern, bool case_less);
  Pattern(const Pattern&) = delete;
  ~Pattern();

public:
  void* db() const { return db_; }

private:
  void compile(const std::string& pattern, bool case_less);
  void compile(const std::string_view pattern, bool case_less);

private:
  void* db_;
};

class PatternList {
public:
  void add(const std::string& pattern, bool case_less, uint64_t id);
  const Pattern* get(uint64_t id) const;

private:
  std::unordered_map<uint64_t, Pattern> pattern_map_;
};
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity