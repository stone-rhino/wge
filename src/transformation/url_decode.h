#pragma once

#include <string>

#include <boost/url.hpp>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class UrlDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(urlDecode);

public:
  std::string evaluate(std::string_view data) const override {
    boost::urls::pct_string_view pct_str(data);
    return pct_str.decode();
  }
};
} // namespace Transformation
} // namespace SrSecurity
