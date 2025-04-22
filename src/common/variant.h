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

#include <format>
#include <string>
#include <string_view>
#include <variant>

namespace SrSecurity {
namespace Common {
using Variant = std::variant<std::monostate, int, std::string_view>;
} // namespace Common
static const Common::Variant EMPTY_VARIANT;
} // namespace SrSecurity

#define IS_EMPTY_VARIANT(variant) std::holds_alternative<std::monostate>(variant)
#define IS_INT_VARIANT(variant) std::holds_alternative<int>(variant)
#define IS_STRING_VIEW_VARIANT(variant) std::holds_alternative<std::string_view>(variant)

#define VISTIT_VARIANT_AS_STRING(variant)                                                          \
  std::visit(                                                                                      \
      [](auto&& arg) -> std::string {                                                              \
        if constexpr (std::is_same_v<std::decay_t<decltype(arg)>, std::monostate>) {               \
          return "monostate";                                                                      \
        } else {                                                                                   \
          std::string result = std::format("{}", arg);                                             \
          if (result.size() > 100) {                                                               \
            result = result.substr(0, 100) + "...";                                                \
          }                                                                                        \
          return result;                                                                           \
        }                                                                                          \
      },                                                                                           \
      variant)
