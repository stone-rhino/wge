#pragma once

#include <string_view>
#include <unordered_set>

namespace SrSecurity {
namespace Variable {
/**
 * Base class for collection variables.
 */
class CollectionBase {
public:
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

protected:
  std::unordered_set<std::string_view> except_variables_;
};
} // namespace Variable
} // namespace SrSecurity