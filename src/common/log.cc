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