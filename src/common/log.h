#pragma once

#include <iostream>

#include <spdlog/spdlog.h>

#define SRSECURITY_LOG_ACTIVE_LEVEL_TRACE 1
#define SRSECURITY_LOG_ACTIVE_LEVEL_DEBUG 2
#define SRSECURITY_LOG_ACTIVE_LEVEL_INFO 3
#define SRSECURITY_LOG_ACTIVE_LEVEL_WARN 4
#define SRSECURITY_LOG_ACTIVE_LEVEL_ERROR 5
#define SRSECURITY_LOG_ACTIVE_LEVEL_CRITICAL 6
#define SRSECURITY_LOG_ACTIVE_LEVEL_OFF 7

// The default log active level is SRSECURITY_LOG_ACTIVE_LEVEL_OFF, which means that all logs are
// not output.
#if !defined(SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_ACTIVE_LEVEL SRSECURITY_LOG_ACTIVE_LEVEL_OFF
#endif

#if (1 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_TRACE(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::trace, __VA_ARGS__);       \
  } while (0)
#else
#define SRSECURITY_LOG_TRACE(...)
#endif

#if (2 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_DEBUG(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::debug, __VA_ARGS__);       \
  } while (0)
#else
#define SRSECURITY_LOG_DEBUG(...)
#endif

#if (3 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_INFO(...)                                                                   \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::info, __VA_ARGS__);        \
  } while (0)
#else
#define SRSECURITY_LOG_INFO(...)
#endif

#if (4 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_WARN(...)                                                                   \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::warn, __VA_ARGS__);        \
  } while (0)
#else
#define SRSECURITY_LOG_WARN(...)
#endif

#if (5 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_ERROR(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::err, __VA_ARGS__);         \
  } while (0)
#else
#define SRSECURITY_LOG_ERROR(...)
#endif

#if (6 >= SRSECURITY_LOG_ACTIVE_LEVEL)
#define SRSECURITY_LOG_CRITICAL(...)                                                               \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(SrSecurity::Common::Log::logger_, spdlog::level::critical, __VA_ARGS__);    \
  } while (0)
#else
#define SRSECURITY_LOG_CRITICAL(...)
#endif

namespace SrSecurity {
namespace Common {
class Log {
public:
  static void init(spdlog::level::level_enum level, const std::string& log_file);

public:
  static spdlog::logger* logger_;

private:
  static std::shared_ptr<spdlog::logger> logger_holder_;
};
} // namespace Common
} // namespace SrSecurity