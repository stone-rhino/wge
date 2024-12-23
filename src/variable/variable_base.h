#pragma once

#include <string>
#include <string_view>

#include "../http_extractor.h"

namespace SrSecurity {
namespace Variable {

/**
 * Base class of variable
 */
class VariableBase {
public:
  VariableBase(std::string&& name, bool is_not) : name_(std::move(name)), is_not_(is_not) {
    initSubName();
  }

public:
  virtual bool evaluate(const HttpExtractor& extractor) const = 0;

public:
  const std::string& name() const { return name_; }
  const std::string_view& subName() const { return sub_name_; }
  bool isNot() const { return is_not_; }

private:
  void initSubName() {
    std::string_view name(name_);
    auto pos = name.find(':');
    if (pos != name.npos) {
      sub_name_ = name.substr(pos + 1);
    }
  }

protected:
  std::string name_;
  std::string_view sub_name_;
  bool is_not_;
};
} // namespace Variable
} // namespace SrSecurity