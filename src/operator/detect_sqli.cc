#include "detect_sqli.h"

#include <libinjection.h>

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
    // TODO(zhouyu 2025-03-26): Add the matched data to the transaction
  }

  return is_sqli;
}
} // namespace Operator
} // namespace SrSecurity