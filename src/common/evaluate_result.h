#pragma once

#include <array>
#include <vector>

#include "assert.h"
#include "empty_string.h"
#include "variant.h"

namespace SrSecurity {
namespace Common {
// The evaluate result.Use for the variable and macro to return the result.
// It is used similar to the SBO(Short Buffer Optimization) to store the result. But it is a little
// different from the traditional SBO. It will try to store the result in the stack first. If the
// result is too big, it will store the result in the heap, but will not move the result from the
// stack to the heap. It use the stack and heap at the same time.
class EvaluateResult {
private:
  // The size of the stack result. About how to set the size of the stack result, we can run the
  // benchmark to get the best size of the stack result. Currnetly, we set the size of the stack
  // result to 1, may be it is not the best size.
  static constexpr size_t stack_result_size = 1;

public:
  EvaluateResult() = default;
  EvaluateResult(const EvaluateResult&) = delete;

public:
  struct Result {
    std::string string_buffer_;
    Common::Variant variant_;
    Result() = default;
    Result(const Common::Variant& value) : variant_(value) {}
    Result(std::string&& value) : string_buffer_(std::move(value)), variant_(string_buffer_) {}
    Result(Result&& result) {
      variant_ = std::move(result.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_) && !result.string_buffer_.empty()) {
        string_buffer_ = std::move(result.string_buffer_);
        variant_ = string_buffer_;
      }
    }
    void operator=(Result&& result) {
      variant_ = std::move(result.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_) && !result.string_buffer_.empty()) {
        string_buffer_ = std::move(result.string_buffer_);
        variant_ = string_buffer_;
      }
    }
  };

public:
  /**
   * Get the front value of the result.
   * @return the front value of the result.
   */
  const Common::Variant& front() const {
    if (size_ != 0) [[likely]] {
      return stack_results_.front().variant_;
    }
    return EMPTY_VARIANT;
  }

  /**
   * Get the value of the result by index.
   * @param index the index of the result.
   * @return the value of the result.
   */
  const Common::Variant& get(size_t index) const {
    assert(index < size_);
    if (index < size_) [[likely]] {
      if (index < stack_result_size) [[likely]] {
        return stack_results_[index].variant_;
      }
      return heap_results_[index - stack_result_size].variant_;
    }
    return EMPTY_VARIANT;
  }

  /**
   * Append the value to the result.
   * @param value the value to append.
   */
  template <class T> void append(T&& value) {
    if (size_ < stack_result_size) [[likely]] {
      stack_results_[size_].variant_ = std::forward<T>(value);
    } else {
      heap_results_.emplace_back(std::forward<T>(value));
    }
    ++size_;
  }

  /**
   * Clear the result.
   */
  const void clear() {
    heap_results_.clear();
    size_ = 0;
  }

  /**
   * Get the size of the result.
   * @return the size of the result.
   */
  size_t size() const { return size_; }

  /**
   * Move the result.
   * @param index the index of the result.
   * @return the result
   */
  Result move(size_t index) {
    assert(index < size_);
    Result result;
    if (index < size_) [[likely]] {
      EvaluateResult::Result* p = index < stack_result_size
                                      ? &stack_results_[index]
                                      : &heap_results_[index - stack_result_size];
      result.variant_ = std::move(p->variant_);
      result.string_buffer_ = std::move(p->string_buffer_);
      if (!result.string_buffer_.empty() && IS_STRING_VIEW_VARIANT(p->variant_)) {
        result.variant_ = result.string_buffer_;
      }
      p->variant_ = EMPTY_VARIANT;
    }
    return result;
  }

private:
  std::array<Result, stack_result_size> stack_results_;
  std::vector<Result> heap_results_;
  size_t size_{0};
};

template <> inline void EvaluateResult::append(std::string&& value) {
  if (size_ < stack_result_size) [[likely]] {
    auto& result = stack_results_[size_];
    result.string_buffer_ = std::move(value);
    result.variant_ = result.string_buffer_;
  } else {
    heap_results_.emplace_back(std::move(value));
  }
  ++size_;
}
} // namespace Common
} // namespace SrSecurity
