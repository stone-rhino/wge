/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
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