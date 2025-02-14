#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

namespace SrSecurity {
namespace Common {
class Duration {
public:
  Duration() : start_clock_(now()) {}

public:
  uint64_t milliseconds() const { return end() - start_clock_; }
  uint64_t seconds() const { return (end() - start_clock_) / 1000; }
  void stop() { end_clock_ = now(); }

private:
  uint64_t now() const {
    using namespace std::chrono;
    return time_point_cast<std::chrono::milliseconds>(steady_clock::now())
        .time_since_epoch()
        .count();
  }

  uint64_t end() const { return end_clock_ ? end_clock_ : now(); }

private:
  uint64_t start_clock_;
  uint64_t end_clock_{0};
};
} // namespace common
} // namespace SrSecurity