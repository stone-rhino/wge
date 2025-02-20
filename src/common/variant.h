#pragma once

#include <string>
#include <string_view>
#include <variant>

namespace SrSecurity {
namespace Common {
using Variant = std::variant<std::monostate, int, std::string, std::string_view>;
} // namespace Common
static const Common::Variant EMPTY_VARIANT;
} // namespace SrSecurity

#define IS_EMPTY_VARIANT(variant) std::holds_alternative<std::monostate>(variant)
#define IS_INT_VARIANT(variant) std::holds_alternative<int>(variant)
#define IS_STRING_VARIANT(variant) std::holds_alternative<std::string>(variant)
#define IS_STRING_VIEW_VARIANT(variant) std::holds_alternative<std::string_view>(variant)