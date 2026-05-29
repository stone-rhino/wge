/**
 * Copyright (c) 2024-2026 Stone Rhino and contributors.
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

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <stdint.h>

#include "pattern.h"
#include "scratch_pool.h"

namespace Wge {
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

  // The scratch pool and per thread scratch space for pcre.
  // We must ensure that the variable type is trivially destructible when using thread_local,
  // otherwise it will delay the unloading of the shared library (until the thread exits). And
  // worker_scratch_ is a pointer, which satisfies the requirement of being trivially destructible,
  // so we can safely use thread_local to store it. At the same time, to ensure that the relevant
  // resources are also properly released after unloading the shared library, we use scratch_pool_
  // to manage the scratch space.
  static ScratchPool scratch_pool_;
  static thread_local Scratch* per_thread_scratch_;
};
} // namespace Pcre
} // namespace Common
} // namespace Wge