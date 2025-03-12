#include "within.h"

namespace SrSecurity {
namespace Operator {
Common::LruCache<int64_t, std::shared_ptr<Common::Hyperscan::HsDataBase>, 101>
    Within::database_cache_(32);
} // namespace Operator
} // namespace SrSecurity