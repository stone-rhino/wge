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
