#pragma once

#include <string_view>
#include <unordered_map>
#include <vector>

namespace SrSecurity {
namespace Common {
namespace Ragel {
class QueryParam {
public:
  void init(std::string_view query_param_str);

public:
  const std::unordered_map<std::string_view, std::string_view>& get() const {
    return query_param_map_;
  }

  const std::vector<std::unordered_map<std::string_view, std::string_view>::iterator>&
  getLinked() const {
    return query_param_linked_;
  }

private:
  std::unordered_map<std::string_view, std::string_view> query_param_map_;
  std::vector<std::unordered_map<std::string_view, std::string_view>::iterator> query_param_linked_;
};
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity