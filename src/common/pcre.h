#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <stdint.h>

namespace SrSecurity {
namespace Common {

class Pcre {
public:
  Pcre(const std::string& pattern, bool case_less);
  Pcre(const Pcre&) = delete;
  ~Pcre();

public:
  /**
   * Scratch space for a match operation.
   * Construct a Scratch object with the number of matches expected.
   */
  struct Scratch {
    void* scratch_;
    Scratch(int matched_count);
    ~Scratch();
  };

public:
  std::vector<std::pair<size_t, size_t>> match(std::string_view subject, Scratch& scratch) const;
  std::vector<std::pair<size_t, size_t>> matchGlobal(std::string_view subject,
                                                     Scratch& scratch) const;

private:
  void compile(const std::string& pattern, bool case_less);

private:
  void* db_;
};

} // namespace Common
} // namespace SrSecurity