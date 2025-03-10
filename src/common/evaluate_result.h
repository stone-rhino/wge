#pragma once

#include "variant.h"

namespace SrSecurity {
namespace Common {

// The evaluate result
// Use for the variable and macro to return the result
class EvaluateResult {
public:
  EvaluateResult() = default;
  EvaluateResult(const EvaluateResult&) = delete;
  void operator=(const EvaluateResult&) = delete;
  void operator=(EvaluateResult&& buffer) {
    variant_ = std::move(buffer.variant_);
    if (IS_STRING_VIEW_VARIANT(variant_)) {
      string_buffer_ = std::move(buffer.string_buffer_);
      variant_ = string_buffer_;
    }
  }
  EvaluateResult(EvaluateResult&& buffer) {
    variant_ = std::move(buffer.variant_);
    if (IS_STRING_VIEW_VARIANT(variant_)) {
      string_buffer_ = std::move(buffer.string_buffer_);
      variant_ = string_buffer_;
    }
  }

public:
  const Common::Variant& get() const { return variant_; }
  void set(int value) { variant_ = value; }
  void set(std::string_view value) { variant_ = value; }
  void set(const std::string& value) { variant_ = value; }
  void set(const Common::Variant& value) { variant_ = value; }
  void set(std::string&& value) {
    string_buffer_ = std::move(value);
    variant_ = string_buffer_;
  }
  const void clear() { variant_ = EMPTY_VARIANT; }

  std::string move() {
    std::string buffer = std::move(string_buffer_);
    if (buffer.empty() && IS_STRING_VIEW_VARIANT(variant_)) {
      buffer = std::get<std::string_view>(variant_);
    }
    variant_ = EMPTY_VARIANT;
    return buffer;
  }

private:
  Common::Variant variant_;
  std::string string_buffer_;
};
} // namespace Common
} // namespace SrSecurity
