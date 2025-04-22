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

#define RETURN_IF_COUNTER(collection_func, speicify_func)                                          \
  if (is_counter_) [[unlikely]] {                                                                  \
    if (sub_name_.empty()) {                                                                       \
      collection_func                                                                              \
    } else {                                                                                       \
      speicify_func                                                                                \
    }                                                                                              \
    return;                                                                                        \
  }

#define RETURN_VALUE(collection_func, collection_regex_func, speicify_func)                        \
  if (sub_name_.empty()) [[unlikely]] {                                                            \
    collection_func                                                                                \
  } else {                                                                                         \
    if (isRegex()) {                                                                               \
      collection_regex_func                                                                        \
    } else {                                                                                       \
      speicify_func                                                                                \
    }                                                                                              \
  }
