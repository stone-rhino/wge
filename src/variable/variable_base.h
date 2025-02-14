#pragma once

#include <string>
#include <string_view>

#include <string.h>

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
 * Base class of variable
 */
class VariableBase {
public:
  struct RegexExpression {
    std::string req_line_;
    std::string req_headers_;
    std::string req_body_;
    std::string resp_line_;
    std::string resp_headers_;
    std::string resp_body_;
  };

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

public:
  virtual const std::string& evaluate(Transaction& t) const {
    static std::string empty_string;
    return empty_string;
  }
  virtual void preCompile() = 0;
  virtual FullName fullName() const = 0;
  virtual const char* mainName() const = 0;

public:
  const std::string& subName() const { return sub_name_; }
  bool isNot() const { return is_not_; }
  bool isCounter() const { return is_counter_; }
  const RegexExpression& regexExpr() const { return regex_expr_; }

private:
protected:
  std::string sub_name_;
  bool is_not_;
  bool is_counter_;
  RegexExpression regex_expr_;
};
} // namespace Variable
} // namespace SrSecurity

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