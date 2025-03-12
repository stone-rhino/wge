#include "within.h"

namespace SrSecurity {
namespace Operator {
std::unordered_map<size_t, std::shared_ptr<Common::Hyperscan::HsDataBase>> Within::database_cache_;
std::mutex Within::database_cache_mutex_;
} // namespace Operator
} // namespace SrSecurity