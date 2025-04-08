#include "escape_seq_decode.h"
#include <escape_seq_decode.h>

namespace SrSecurity {
namespace Transformation {
std::string EscapeSeqDecode::evaluate(std::string_view data) const {
  return escapeSeqDecode(data);
}
} // namespace Transformation
} // namespace SrSecurity
