#include "engine.h"

#include "antlr4/parser.h"
#include "common/assert.h"
#include "common/likely.h"

std::thread::id main_thread_id;

namespace SrSecurity {
Engine::Engine() : parser_(std::make_shared<Antlr4::Parser>()) {
  // We assume that it can only be constructed in the main thread
  main_thread_id = std::this_thread::get_id();
}

std::expected<bool, std::string> Engine::loadFromFile(const std::string& file_path) {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return parser_->loadFromFile(file_path);
}

std::expected<bool, std::string> Engine::load(const std::string& directive) {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  return parser_->load(directive);
}

void Engine::preEvaluateRules() {
  // An efficient and rational design should not call this method in the worker thread.
  // This assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  initValidRules();
}

TransactionPtr Engine::makeTransaction() const {
  return std::unique_ptr<Transaction>(new Transaction(*this));
}

void Engine::initValidRules() {}

} // namespace SrSecurity