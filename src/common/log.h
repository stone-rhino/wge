#pragma once

#include <spdlog/spdlog.h>

#ifndef SRSECURITY_LOG
#define SRSECURITY_LOG(LEVEL, ...)                                                                 \
  SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::LEVEL, __VA_ARGS__)
#endif

namespace SrSecurity {
namespace common {
class Log {
public:
  // log_level shoud be in [trace debug info warn err critical]
  static void init(const std::string& log_level, const std::string& log_file);

private:
  static std::unordered_map<std::string, spdlog::level::level_enum> level_table_;
};
} // namespace common
} // namespace SrSecurity