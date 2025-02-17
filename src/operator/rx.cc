#include "rx.h"

namespace SrSecurity {
namespace Operator {
thread_local Common::Pcre::Scratch Rx::per_thread_pcre_scratch_(99);
}
} // namespace SrSecurity