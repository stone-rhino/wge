#pragma once

#include <string>

namespace SrSecurity {
namespace Common {
namespace File {
/**
 * Make a absolute file path from the rule file path and the file path.
 * @param rule_file_path the path of the rule file that the engine is currently loading.
 * @param file_path the path of the file that the right-value of the operator is pointing to.
 * @return the absolute file path.
 */
std::string makeFilePath(std::string_view rule_file_path, const std::string& file_path);
} // namespace File
} // namespace Common
} // namespace SrSecurity