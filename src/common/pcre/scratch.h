#pragma once

namespace SrSecurity {
namespace Common {
namespace Pcre {
class Scratch {
public:
  Scratch(int matched_count);
  ~Scratch();

public:
  void* hanlde() const { return scratch_; }

private:
  void* scratch_;
};
} // namespace Pcre
} // namespace Common
} // namespace SrSecurity
