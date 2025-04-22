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

#include <string_view>
#include <unordered_set>

#include "../common/pcre/scanner.h"

namespace Wge {
namespace Variable {
/**
 * Base class for collection variables.
 */
class CollectionBase {
public:
  CollectionBase(const std::string& sub_name) {
    if (sub_name.size() >= 3 && sub_name.front() == '/' && sub_name.back() == '/') {
      pcre_scanner_ = std::make_unique<Common::Pcre::Scanner>(
          std::string_view{sub_name.data() + 1, sub_name.size() - 2}, false, false);
    }
  }
  virtual ~CollectionBase() = default;

public:
  /**
   * Add a variable to the exception list.
   * @param variable_sub_name the sub name of the variable.
   */
  void addExceptVariable(std::string_view variable_sub_name) {
    except_variables_.insert(variable_sub_name);
  }

  /**
   * Check whether the variable is in the exception list.
   * @param variable_sub_name the sub name of the variable.
   * @return true if the variable is in the exception list, false otherwise.
   */
  bool hasExceptVariable(std::string_view variable_sub_name) const {
    return except_variables_.find(variable_sub_name) != except_variables_.end();
  }

  bool isRegex() const { return pcre_scanner_ != nullptr; }

  bool match(std::string_view subject) const {
    if (pcre_scanner_) {
      return pcre_scanner_->match(subject);
    }

    return false;
  }

protected:
  std::unordered_set<std::string_view> except_variables_;

private:
  std::unique_ptr<Common::Pcre::Scanner> pcre_scanner_;
};
} // namespace Variable
} // namespace Wge