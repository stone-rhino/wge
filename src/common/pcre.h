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
  class Scratch {
    friend class Pcre;

  public:
    Scratch();
    ~Scratch();

  private:
    void* scratch_;
  };

public:
  bool match(std::string_view subject, Scratch& scratch, size_t& from, size_t& to) const;
  std::vector<std::pair<size_t, size_t>> matchGlobal(std::string_view subject,
                                                     Scratch& scratch) const;

private:
  void compile(const std::string& pattern, bool case_less);

private:
  void* db_;
};

} // namespace Common
} // namespace SrSecurity