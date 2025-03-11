#include "file.h"

#include <filesystem>

namespace SrSecurity {
namespace Common {
namespace File {
std::string makeFilePath(std::string_view rule_file_path, const std::string& file_path) {
  // If the file path is already an absolute path, return it.
  if (std::filesystem::path(file_path).is_absolute()) {
    return file_path;
  }

  // If the file path is a relative path, return the absolute path.
  return std::filesystem::path(rule_file_path).parent_path().append(file_path).string();
}
} // namespace File
} // namespace Common
} // namespace SrSecurity