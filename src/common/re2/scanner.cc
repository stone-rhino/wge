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
#include "scanner.h"

namespace Wge {
namespace Common {
namespace Re2 {
Scanner::Scanner(const std::string& pattern, bool case_less, bool captrue)
    : Scanner(std::string_view(pattern), case_less, captrue) {}

Scanner::Scanner(std::string_view pattern, bool case_less, bool captrue) {
  RE2::Options options;
  options.set_case_sensitive(!case_less);
  options.set_never_capture(!captrue);
  re2_ = std::make_unique<RE2>(pattern, options);
}

void Scanner::match(std::string_view subject,
                    std::vector<std::pair<size_t, size_t>>& result) const {
  int groups = re2_->NumberOfCapturingGroups();
  std::vector<re2::StringPiece> submatch(groups + 1); // +1 for full match

  bool matched =
      re2_->Match(subject, 0, subject.size(), RE2::UNANCHORED, submatch.data(), submatch.size());

  if (matched) {
    for (size_t i = 0; i < submatch.size(); ++i) {
      size_t from = submatch[i].data() ? (submatch[i].data() - subject.data()) : 0;
      size_t to = from + submatch[i].size();
      result.emplace_back(from, to);
    }
  }
}
} // namespace Re2
} // namespace Common
} // namespace Wge