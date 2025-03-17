#pragma once

#include <string>

#include <boost/url.hpp>

#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
class UrlDecode : public TransformBase {
  DECLARE_TRANSFORM_NAME(urlDecode);

public:
  std::string evaluate(const void* data, size_t data_len) const override {
    boost::urls::pct_string_view pct_str(reinterpret_cast<const char*>(data), data_len);
    return pct_str.decode();
  }
};
} // namespace Transformation
} // namespace SrSecurity
