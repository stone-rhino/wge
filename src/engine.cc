#include "engine.h"

#include "common/assert.h"
#include "common/likely.h"
#include "parser.h"

std::thread::id main_thread_id;

namespace SrSecurity {
Engine::Engine() {
  // we assume that it can only be constructed in the main thread
  main_thread_id = std::this_thread::get_id();
}

void Engine::loadFromFile(const std::string& file_path) {
  // an efficient and rational design should not call this method in the worker thread.
  // this assert check that this method can only be called in the main thread
  ASSERT_IS_MAIN_THREAD();

  parser_.loadFromFile(file_path);
}

TransactionSharedPtr Engine::makeTransaction() const {
  return std::shared_ptr<Transaction>(new Transaction(*this));
}

} // namespace SrSecurity