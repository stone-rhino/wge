#include "pm_from_file.h"

namespace SrSecurity {
namespace Operator {
std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
    PmFromFile::database_cache_;
} // namespace Operator
} // namespace SrSecurity