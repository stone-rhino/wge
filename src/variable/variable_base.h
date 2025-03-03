#pragma once

#include <string>
#include <string_view>

#include <string.h>

#include "../common/variant.h"
#include "../http_extractor.h"
#include "../transaction.h"

#define DECLARE_VIRABLE_NAME(name)                                                                 \
public:                                                                                            \
  FullName fullName() const override { return {main_name_, sub_name_}; }                           \
  const char* mainName() const override { return main_name_; }                                     \
                                                                                                   \
private:                                                                                           \
  static constexpr char main_name_[] = #name;

namespace SrSecurity {
namespace Variable {

/**
 * Base class for all variables.
 */
class VariableBase {
public:
  struct FullName {
    const char* main_name_;
    const std::string& sub_name_;

    bool operator>(const FullName& full_name) const {
      int result = ::strcmp(main_name_, full_name.main_name_);
      if (result == 0) {
        return sub_name_ > full_name.sub_name_;
      }

      return result > 0;
    }

    bool operator<(const FullName& full_name) const {
      int result = ::strcmp(main_name_, full_name.main_name_);
      if (result == 0) {
        return sub_name_ < full_name.sub_name_;
      }

      return result < 0;
    }

    bool operator==(const FullName& full_name) const {
      int result = ::strcmp(main_name_, full_name.main_name_);
      if (result == 0) {
        return sub_name_ == full_name.sub_name_;
      }

      return false;
    }
  };

public:
  VariableBase(std::string&& sub_name, bool is_not, bool is_counter)
      : sub_name_(std::move(sub_name)), is_not_(is_not), is_counter_(is_counter) {}
  VariableBase(bool is_not, bool is_counter) : is_not_(is_not), is_counter_(is_counter) {}
  virtual ~VariableBase() = default;

public:
  /**
   * Evaluate the variable.
   * @param t the transaction.
   * @return the value of the variable.
   * @note The result of the variable evaluation is stored in the transaction's evaluated buffer. In
   * each transaction, all variables share the same evaluated buffer, so we need to copy it to a
   * local variable if we want to use it after the next variable object is evaluated.
   */
  virtual const Common::Variant& evaluate(Transaction& t) const = 0;

  /**
   * Get the full name of the variable.
   * @return the full name of the variable.
   */
  virtual FullName fullName() const = 0;

  /**
   * Get the main name of the variable.
   * @return the main name of the variable.
   */
  virtual const char* mainName() const = 0;

public:
  /**
   * Get the sub name of the variable.
   * @return the sub name of the variable.
   */
  const std::string& subName() const { return sub_name_; }

  /**
   * Get whether the variable is negated.
   * @return true if the variable is negated, false otherwise.
   */
  bool isNot() const { return is_not_; }

  /**
   * Get whether the variable is a counter.
   * @return true if the variable is a counter, false otherwise.
   */
  bool isCounter() const { return is_counter_; }

protected:
  std::string sub_name_;
  bool is_not_;
  bool is_counter_;
};
} // namespace Variable
} // namespace SrSecurity

/**
 * Hash function for FullName.
 */
namespace std {
template <> struct hash<SrSecurity::Variable::VariableBase::FullName> {
  size_t operator()(const SrSecurity::Variable::VariableBase::FullName& s) const {
    size_t h1 = 0;
    const char* p = s.main_name_;
    while (p != nullptr && *p != '\0') {
      h1 = h1 * 131 + *p;
      ++p;
    }
    size_t h2 = std::hash<std::string>()(s.sub_name_);
    return h1 ^ (h2 << 1);
  }
};
} // namespace std