#include "variable_base.h"

namespace SrSecurity {
namespace Variable {
thread_local Common::Variant VariableBase::variant_value_;
};
} // namespace SrSecurity