#include "rx.h"

namespace SrSecurity {
namespace Operator {
std::forward_list<std::string> Rx::macro_value_cache_;
std::unordered_map<std::string_view, std::unique_ptr<Common::Pcre::Scanner>> Rx::macro_pcre_cache_;
std::mutex Rx::macro_chche_mutex_;
} // namespace Operator
} // namespace SrSecurity