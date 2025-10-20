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
#include "pm_from_file.h"

namespace Wge {
namespace Operator {
std::unordered_map<std::string, std::shared_ptr<Common::Hyperscan::HsDataBase>>
    PmFromFile::database_cache_;
std::mutex PmFromFile::database_cache_mutex_;

void PmFromFile::init(const std::string& serialize_dir) {
  // Make the file path absolute.
  std::string file_path = Common::File::makeFilePath(curr_rule_file_path_, literal_value_);

  // Load the hyperscan database and create a scanner.
  // We cache the hyperscan database to avoid loading(complie) the same database multiple times.
  std::unique_lock<std::mutex> locker(database_cache_mutex_);
  auto iter = database_cache_.find(file_path);
  if (iter == database_cache_.end()) {
    locker.unlock();
    const char* serialize_dir_cstr = serialize_dir.empty() ? nullptr : serialize_dir.c_str();
    auto hs_db = std::make_shared<Common::Hyperscan::HsDataBase>(std::move(expression_list_), false,
                                                                 serialize_dir_cstr);
    scanner_ = std::make_unique<Common::Hyperscan::Scanner>(hs_db);
    locker.lock();
    database_cache_.emplace(file_path, hs_db);
  } else {
    scanner_ = std::make_unique<Common::Hyperscan::Scanner>(iter->second);
  }
}
} // namespace Operator
} // namespace Wge