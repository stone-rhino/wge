#pragma once
#include <memory>
#include <unordered_set>

#include "../common/assert.h"

namespace SrSecurity {
namespace Rule {
class RuleBase {
public:
  uint64_t id() const { return id_; }

  bool hasTag(const std::string& tag) const {
    // an efficient and rational design should not call this method in the worker thread.
    // this assert check that this method can only be called in the main thread
    ASSERT_IS_MAIN_THREAD();

    return tags_.find(tag) != tags_.end();
  }

  bool hasTag(const std::unordered_set<std::string>& tags) const {
    // an efficient and rational design should not call this method in the worker thread.
    // this assert check that this method can only be called in the main thread
    ASSERT_IS_MAIN_THREAD();

    for (auto& tag : tags) {
      if (hasTag(tag)) {
        return true;
      }
    }

    return false;
  }

private:
  uint64_t id_;
  std::unordered_set<std::string> tags_;
};
using RuleSharedPtr = std::shared_ptr<RuleBase>;
} // namespace Rule
} // namespace SrSecurity