/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <iostream>

#include <spdlog/spdlog.h>

#define WGE_LOG_ACTIVE_LEVEL_TRACE 1
#define WGE_LOG_ACTIVE_LEVEL_DEBUG 2
#define WGE_LOG_ACTIVE_LEVEL_INFO 3
#define WGE_LOG_ACTIVE_LEVEL_WARN 4
#define WGE_LOG_ACTIVE_LEVEL_ERROR 5
#define WGE_LOG_ACTIVE_LEVEL_CRITICAL 6
#define WGE_LOG_ACTIVE_LEVEL_OFF 7

// The default log active level is WGE_LOG_ACTIVE_LEVEL_OFF, which means that all logs are
// not output.
#if !defined(WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_ACTIVE_LEVEL WGE_LOG_ACTIVE_LEVEL_OFF
#endif

#if (1 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_TRACE(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::trace, __VA_ARGS__);       \
  } while (0)
#else
#define WGE_LOG_TRACE(...)
#endif

#if (2 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_DEBUG(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::debug, __VA_ARGS__);       \
  } while (0)
#else
#define WGE_LOG_DEBUG(...)
#endif

#if (3 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_INFO(...)                                                                   \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::info, __VA_ARGS__);        \
  } while (0)
#else
#define WGE_LOG_INFO(...)
#endif

#if (4 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_WARN(...)                                                                   \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::warn, __VA_ARGS__);        \
  } while (0)
#else
#define WGE_LOG_WARN(...)
#endif

#if (5 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_ERROR(...)                                                                  \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::err, __VA_ARGS__);         \
  } while (0)
#else
#define WGE_LOG_ERROR(...)
#endif

#if (6 >= WGE_LOG_ACTIVE_LEVEL)
#define WGE_LOG_CRITICAL(...)                                                               \
  do {                                                                                             \
    SPDLOG_LOGGER_CALL(Wge::Common::Log::logger_, spdlog::level::critical, __VA_ARGS__);    \
  } while (0)
#else
#define WGE_LOG_CRITICAL(...)
#endif

namespace Wge {
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
} // namespace Wge