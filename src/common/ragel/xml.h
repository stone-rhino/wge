#pragma once

#include <string_view>
#include <vector>

namespace SrSecurity {
namespace Common {
namespace Ragel {
class Xml {
public:
  void init(std::string_view xml_str);

public:
  const std::vector<std::string_view>& getAttrValues() const { return attr_values_; }
  const std::vector<std::string_view>& getTagValues() const { return tag_values_; }

  void clear() {
    attr_values_.clear();
    tag_values_.clear();
  }
private:
  std::vector<std::string_view> attr_values_;
  std::vector<std::string_view> tag_values_;
};
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity