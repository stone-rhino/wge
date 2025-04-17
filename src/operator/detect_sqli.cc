#include "detect_sqli.h"

#include <libinjection.h>

#include "../common/evaluate_result.h"

namespace SrSecurity {
namespace Operator {
bool DetectSqli::evaluate(Transaction& t, const Common::Variant& operand) const {
  if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
    return false;
  }

  std::string_view data = std::get<std::string_view>(operand);
  char fingerprint[8];
  bool is_sqli = libinjection_sqli(data.data(), data.size(), fingerprint) != 0;
  if (is_sqli) {
    Common::EvaluateResults::Element value;
    value.string_buffer_ = std::string(fingerprint, sizeof(fingerprint));
    value.variant_ = value.string_buffer_;
    t.addCapture(std::move(value));
  }

  return is_sqli;
}
} // namespace Operator
} // namespace SrSecurity