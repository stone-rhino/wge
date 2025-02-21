#include "log.h"

#include <spdlog/sinks/rotating_file_sink.h>

namespace SrSecurity {
namespace Common {
spdlog::logger* Log::logger_ = spdlog::default_logger_raw();
std::shared_ptr<spdlog::logger> Log::logger_holder_;

void Log::init(spdlog::level::level_enum level, const std::string& log_file) {
  if (!log_file.empty()) {
    logger_holder_ = spdlog::rotating_logger_mt("rotating_logger", "srsecurity/srsecurity.log",
                                                1024 * 1024 * 100, 3);
    logger_ = logger_holder_.get();
  }

  spdlog::set_level(level);
  if (level != spdlog::level::off) {
    spdlog::flush_on(level);
  }

  spdlog::set_pattern("[SRSECURITY][%Y-%m-%d %H:%M:%S.%e][%t][%^%l%$] %v");
}
} // namespace Common
} // namespace SrSecurity