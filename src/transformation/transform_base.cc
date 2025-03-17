#include "transform_base.h"

namespace SrSecurity {
namespace Transformation {
bool TransformBase::evaluate(Transaction& t, Common::EvaluateResults::Element& data) const {
  std::string result = evaluate(std::get<std::string_view>(data.variant_));
  if (!result.empty()) {
    data.string_buffer_ = std::move(result);
    data.variant_ = data.string_buffer_;
    return true;
  }
  return false;
}
} // namespace Transformation
} // namespace SrSecurity
