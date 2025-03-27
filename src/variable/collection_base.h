#pragma once

#include <string_view>
#include <unordered_set>

#include "../common/pcre/scanner.h"

namespace SrSecurity {
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
} // namespace SrSecurity