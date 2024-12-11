#pragma once

#include <memory>

#include "parser.h"
#include "transaction.h"

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
  Parser parser_;
};
} // namespace SrSecurity