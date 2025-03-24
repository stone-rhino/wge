#include "cmd_line.h"
#include <cmd_line.h>

namespace SrSecurity {
namespace Transformation {
std::string CmdLine::evaluate(std::string_view data) const {
  return cmdLine(data);
}
} // namespace Transformation
} // namespace SrSecurity