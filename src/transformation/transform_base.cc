#include "transform_base.h"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace SrSecurity {
namespace Transformation {
bool TransformBase::evaluate(Transaction& t, const Variable::VariableBase* variable,
                             Common::EvaluateResults::Element& data) const {
  // We don't always use the cache, because the transformation may be too simple, and it's perfmance
  // than search the cache in the hash table.
  // The threshold is 32 bytes, may be it's not a good choice, but it's a good start.
  // TODO(zhouyu 2025-04-11): We can use a better algorithm to choose the threshold.
  constexpr size_t use_cache_limit = 32;
  bool use_cache = std::get<std::string_view>(data.variant_).size() > use_cache_limit;

  if (use_cache) [[likely]] {
    assert(variable);
    Variable::FullName variable_full_name = variable->fullName();
    if (variable->isCollection()) [[likely]] {
      variable_full_name.sub_name_ = data.variable_sub_name_;
    }

    // Check the cache
    std::unordered_map<
        Variable::FullName,
        std::unordered_map<const char*, std::optional<Common::EvaluateResults::Element>>>&
        transform_cache = t.getTransformCache();
    auto iter_variable_full_name = transform_cache.find(variable_full_name);
    if (iter_variable_full_name != transform_cache.end()) {
      auto iter_transform_result = iter_variable_full_name->second.find(name());
      if (iter_transform_result != iter_variable_full_name->second.end()) {
        SRSECURITY_LOG_TRACE("transform cache hit: {} {}", variable_full_name.tostring(), name());

        // The transformation has been evaluated before.
        if (iter_transform_result->second.has_value()) [[likely]] {
          data.variant_ = iter_transform_result->second.value().variant_;
          return true;
        } else {
          return false;
        }
      }
    } else {
      iter_variable_full_name =
          transform_cache
              .emplace(variable_full_name,
                       std::unordered_map<const char*,
                                          std::optional<Common::EvaluateResults::Element>>{})
              .first;

      iter_variable_full_name->second.reserve(16);
    }

    // Evaluate the transformation and store the result in the cache
    std::string buffer;
    bool ret = evaluate(std::get<std::string_view>(data.variant_), buffer);
    if (ret) {
      auto iter_transform_result =
          iter_variable_full_name->second.emplace(name(), Common::EvaluateResults::Element()).first;
      Common::EvaluateResults::Element& result = iter_transform_result->second.value();
      result.string_buffer_ = std::move(buffer);
      result.variant_ = result.string_buffer_;
      data.variant_ = result.variant_;
    } else {
      auto iter_transform_result =
          iter_variable_full_name->second.emplace(name(), std::nullopt).first;
    }

    return ret;
  } else {
    std::string buffer;
    bool ret = evaluate(std::get<std::string_view>(data.variant_), buffer);
    if (ret) {
      data.string_buffer_ = std::move(buffer);
      data.variant_ = data.string_buffer_;
    }

    return ret;
  }
}
} // namespace Transformation
} // namespace SrSecurity
