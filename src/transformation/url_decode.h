#pragma once

#include <string>

#include <boost/url.hpp>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class UrlDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(urlDecode);

public:
  bool evaluate(std::string_view data, std::string& result) const override {
    boost::urls::pct_string_view pct_str(data);
    result = pct_str.decode();
    return true;
  }
};
} // namespace Transformation
} // namespace SrSecurity
