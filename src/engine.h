#pragma once

#include <memory>

#include "transaction.h"

namespace SrSecurity::Parser {
class Parser;
}

namespace SrSecurity {
class Engine {
public:
  Engine();

public:
  /**
   * load the rule set from a file
   * @param file_path supports relative and absolute path
   * @result an error string is returned if fails, an empty string is returned otherwise
   */
  std::string loadFromFile(const std::string& file_path);

  /**
   * load the rule set from a configuration directive
   * @param directive configuration directive
   * @result an error string is returned if fails, an empty string is returned otherwise
   */
  std::string load(const std::string& directive);

public:
  TransactionSharedPtr makeTransaction() const;

private:
  std::unique_ptr<Parser::Parser> parser_;
};
} // namespace SrSecurity