#pragma once

#include <array>
#include <map>
#include <regex>
#include <string>

#include "transform_base.h"

#include "../common/assert.h"

namespace SrSecurity {
namespace Transformation {
class HtmlEntityDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(htmlEntityDecode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    // Regex search the html entity by html_entity_regex_ and replace it with the html_entitys_.
    std::string result;
    std::cmatch cm;
    std::string_view data_view(static_cast<const char*>(data), data_len);
    std::regex_search(data_view.data(), cm, html_entity_regex_);
    size_t last_pos = 0;
    while (!cm.empty()) {
      result.append(data_view.substr(last_pos, cm.position()));
      for (size_t i = 1; i < cm.size(); ++i) {
        if (cm[i].matched) {
          result.push_back(html_entitys_[i - 1].second);
          last_pos = cm.position() + cm.length();
          break;
        }
      }
      std::regex_search(data_view.data() + last_pos, cm, html_entity_regex_);
    }
    return result;
  }

private:
  static const std::array<std::pair<std::string, char>, 6> html_entitys_;
  static std::regex html_entity_regex_;
};
} // namespace Transformation
} // namespace SrSecurity
