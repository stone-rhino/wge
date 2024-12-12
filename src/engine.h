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
  void loadFromFile(const std::string& file_path);
  void load(const std::string& cmd);

public:
  TransactionSharedPtr makeTransaction() const;

private:
  std::unique_ptr<Parser::Parser> parser_;
};
} // namespace SrSecurity