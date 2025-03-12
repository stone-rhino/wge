#pragma once

#include <vector>

#include "empty_string.h"
#include "variant.h"

namespace SrSecurity {
namespace Common {

// The evaluate result
// Use for the variable and macro to return the result
class EvaluateResult {
public:
  EvaluateResult() {
    // Although the count of the result is almost 1, we reserve 8 to avoid the reallocation when
    // some variable return multiple results.
    results_.reserve(8);
  }
  EvaluateResult(const EvaluateResult&) = delete;
  void operator=(const EvaluateResult&) = delete;
  void operator=(EvaluateResult&& buffer) { results_ = std::move(buffer.results_); }
  EvaluateResult(EvaluateResult&& buffer) { results_ = std::move(buffer.results_); }

public:
  /**
   * Get the front value of the result.
   * @return the front value of the result.
   */
  const Common::Variant& front() const {
    if (!results_.empty()) [[likely]] {
      return results_.front().variant_;
    }
    return EMPTY_VARIANT;
  }

  /**
   * Get the value of the result by index.
   * @param index the index of the result.
   * @return the value of the result.
   */
  const Common::Variant& get(size_t index) const {
    assert(index < results_.size());
    if (index < results_.size()) [[likely]] {
      return results_[index].variant_;
    }
    return EMPTY_VARIANT;
  }

  /**
   * Append the value to the result.
   * @param value the value to append.
   */
  void append(int value) { results_.emplace_back(value); }
  void append(std::string_view value) { results_.emplace_back(value); }
  void append(const std::string& value) { results_.emplace_back(value); }
  void append(const Common::Variant& value) { results_.emplace_back(value); }
  void append(std::string&& value) { results_.emplace_back(std::move(value)); }

  /**
   * Clear the result.
   */
  const void clear() { results_.clear(); }

  /**
   * Get the size of the result.
   * @return the size of the result.
   */
  size_t size() const { return results_.size(); }

  /**
   * Move the string buffer of the result by index.
   * @param index the index of the result.
   * @return the string buffer of the result.
   */
  std::string moveString(size_t index) {
    assert(index < results_.size());
    if (index < results_.size()) {
      auto& result = results_[index];
      std::string buffer = std::move(result.string_buffer_);
      if (buffer.empty() && IS_STRING_VIEW_VARIANT(result.variant_)) {
        buffer = std::get<std::string_view>(result.variant_);
      }
      result.variant_ = EMPTY_VARIANT;
      return buffer;
    }
    return EMPTY_STRING;
  }

private:
  struct Result {
    std::string string_buffer_;
    Common::Variant variant_;
    Result(int value) : variant_(value) {}
    Result(std::string_view value) : variant_(value) {}
    Result(const std::string& value) : variant_(value) {}
    Result(const Common::Variant& value) : variant_(value) {}
    Result(std::string&& value) : string_buffer_(std::move(value)), variant_(string_buffer_) {}
    Result(Result&& result) {
      variant_ = std::move(result.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_) && !string_buffer_.empty()) {
        string_buffer_ = std::move(result.string_buffer_);
        variant_ = string_buffer_;
      }
    }
    void operator=(Result&& result) {
      variant_ = std::move(result.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_)) {
        string_buffer_ = std::move(result.string_buffer_);
        variant_ = string_buffer_;
      }
    }
  };

  // Some variable may return multiple strings, so we use a vector to store the result.
  std::vector<Result> results_;
};
} // namespace Common
} // namespace SrSecurity
