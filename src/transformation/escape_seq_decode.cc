#include "escape_seq_decode.h"

#include <escape_seq_decode.h>

namespace SrSecurity {
namespace Transformation {
bool EscapeSeqDecode::evaluate(std::string_view data, std::string& result) const {
  return escapeSeqDecode(data, result);
}
} // namespace Transformation
} // namespace SrSecurity
