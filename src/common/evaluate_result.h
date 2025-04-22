/**
 * Copyright (c) 2024-2025 Stone Rhino and contributors.
 *
 * MIT License (http://opensource.org/licenses/MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <array>
#include <vector>

#include "assert.h"
#include "empty_string.h"
#include "variant.h"

namespace Wge {
namespace Common {
// The evaluate result.Use for the variable and macro to return the result.
// It is used similar to the SBO(Short Buffer Optimization) to store the result. But it is a little
// different from the traditional SBO. It will try to store the result in the stack first. If the
// result is too big, it will store the result in the heap, but will not move the result from the
// stack to the heap. It use the stack and heap at the same time.
class EvaluateResults {
private:
  // The size of the stack result. About how to set the size of the stack result, we can run the
  // benchmark to get the best size of the stack result. Currnetly, we set the size of the stack
  // result to 1, may be it is not the best size.
  static constexpr size_t stack_result_size = 1;

public:
  EvaluateResults() = default;
  EvaluateResults(const EvaluateResults&) = delete;

public:
  struct Element {
    std::string string_buffer_;
    Common::Variant variant_;
    std::string_view variable_sub_name_;
    Element() = default;
    Element(const Common::Variant& value, std::string_view variable_sub_name)
        : variant_(value), variable_sub_name_(variable_sub_name) {}
    Element(std::string&& value, std::string_view variable_sub_name)
        : string_buffer_(std::move(value)), variant_(string_buffer_),
          variable_sub_name_(variable_sub_name) {}
    Element(Element&& element) {
      variant_ = std::move(element.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_) && !element.string_buffer_.empty()) {
        string_buffer_ = std::move(element.string_buffer_);
        variant_ = string_buffer_;
      }
      variable_sub_name_ = element.variable_sub_name_;
    }
    void operator=(const Element& element) {
      variant_ = element.variant_;
      if (IS_STRING_VIEW_VARIANT(variant_) && !element.string_buffer_.empty()) {
        string_buffer_ = element.string_buffer_;
        variant_ = string_buffer_;
      }
      variable_sub_name_ = element.variable_sub_name_;
    }
    void operator=(Element&& element) {
      variant_ = std::move(element.variant_);
      if (IS_STRING_VIEW_VARIANT(variant_) && !element.string_buffer_.empty()) {
        string_buffer_ = std::move(element.string_buffer_);
        variant_ = string_buffer_;
      }
      variable_sub_name_ = element.variable_sub_name_;
    }
  };

public:
  /**
   * Get the front element of the results.
   * @return the front element of the results.
   */
  Element& front() { return stack_results_[0]; }

  /**
   * Get the element of the results by index.
   * @param index the index of the results.
   * @return the element of the results.
   */
  Element& get(size_t index) {
    assert(index < size_);
    if (index < size_) [[likely]] {
      if (index < stack_result_size) [[likely]] {
        return stack_results_[index];
      }
      return heap_results_[index - stack_result_size];
    }
    return stack_results_[0];
  }

  /**
   * Append the value to the result.
   * @param value the value to append.
   */
  template <class T> void append(T&& value, std::string_view variable_sub_name = "") {
    if (size_ < stack_result_size) [[likely]] {
      stack_results_[size_].variant_ = std::forward<T>(value);
      stack_results_[size_].variable_sub_name_ = variable_sub_name;
    } else {
      heap_results_.emplace_back(std::forward<T>(value), variable_sub_name);
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
   * Reserve the size of the result.
   * @param size the size of the result.
   * @note The size is the size of the heap result. The stack result size is fixed.
   */
  void reserve(size_t size) {
    if (size > stack_result_size) [[likely]] {
      heap_results_.reserve(size - stack_result_size);
    }
  }

  /**
   * Move the result.
   * @param index the index of the result.
   * @return the result
   */
  Element move(size_t index) {
    assert(index < size_);
    Element element;
    if (index < size_) [[likely]] {
      EvaluateResults::Element* p = index < stack_result_size
                                        ? &stack_results_[index]
                                        : &heap_results_[index - stack_result_size];
      element.variable_sub_name_ = p->variable_sub_name_;
      element.variant_ = std::move(p->variant_);
      element.string_buffer_ = std::move(p->string_buffer_);
      if (!element.string_buffer_.empty() && IS_STRING_VIEW_VARIANT(p->variant_)) {
        element.variant_ = element.string_buffer_;
      }
      p->variant_ = EMPTY_VARIANT;
    }
    return element;
  }

private:
  std::array<Element, stack_result_size> stack_results_;
  std::vector<Element> heap_results_;
  size_t size_{0};
};

template <>
inline void EvaluateResults::append(std::string&& value, std::string_view variable_sub_name) {
  if (size_ < stack_result_size) [[likely]] {
    auto& result = stack_results_[size_];
    result.string_buffer_ = std::move(value);
    result.variant_ = result.string_buffer_;
  } else {
    heap_results_.emplace_back(std::move(value), variable_sub_name);
  }
  ++size_;
}
} // namespace Common
} // namespace Wge
