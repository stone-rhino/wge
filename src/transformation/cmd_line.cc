#include "cmd_line.h"

#include <cmd_line.h>

namespace SrSecurity {
namespace Transformation {
bool CmdLine::evaluate(std::string_view data, std::string& result) const {
  return cmdLine(data, result);
}
} // namespace Transformation
} // namespace SrSecurity