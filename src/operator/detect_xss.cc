#include "detect_xss.h"

#include <libinjection.h>

namespace SrSecurity {
namespace Operator {
bool DetectXSS::evaluate(Transaction& t, const Common::Variant& operand) const {
  if (!IS_STRING_VIEW_VARIANT(operand)) [[unlikely]] {
    return false;
  }

  std::string_view data = std::get<std::string_view>(operand);
  return libinjection_xss(data.data(), data.size()) != 0;
}
} // namespace Operator
} // namespace SrSecurity