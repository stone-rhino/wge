#include "log.h"

#include <spdlog/sinks/rotating_file_sink.h>

namespace SrSecurity {
namespace common {
std::unordered_map<std::string, spdlog::level::level_enum> Log::level_table_ = {
    {"trace", spdlog::level::trace}, {"debug", spdlog::level::debug},
    {"info", spdlog::level::info},   {"warn", spdlog::level::warn},
    {"err", spdlog::level::err},     {"critical", spdlog::level::critical}};

void Log::init(const std::string& log_level, const std::string& log_file) {
  if (!log_file.empty()) {
    auto rotating_logger =
        spdlog::rotating_logger_mt("rotating_logger", "log/srsecurity.log", 1024 * 1024 * 100, 3);
    spdlog::set_default_logger(rotating_logger);
  }

  auto iter = level_table_.find(log_level);
  if (iter != level_table_.end()) {
    spdlog::set_level(iter->second);
    spdlog::flush_on(iter->second);
  } else {
    spdlog::set_level(spdlog::level::off);
  }

  spdlog::set_pattern("[srsecurity][%Y-%m-%d %H:%M:%S.%e %z][thread %t][%^%l%$] %v");
}
} // namespace common
} // namespace SrSecurity