#include "transform_base.h"

#include "../common/log.h"
#include "../variable/variable_base.h"

namespace SrSecurity {
namespace Transformation {
bool TransformBase::evaluate(Transaction& t, const Variable::VariableBase* variable,
                             Common::EvaluateResults::Element& data) const {
  assert(variable);
  Variable::FullName variable_full_name = variable->fullName();
  if (variable->isCollection()) [[likely]] {
    variable_full_name.sub_name_ = data.variable_sub_name_;
  }

  // Check the cache
  std::unordered_map<Variable::FullName,
                     std::unordered_map<const char*, Common::EvaluateResults::Element>>&
      transform_cache = t.getTransformCache();
  auto iter_variable_full_name = transform_cache.find(variable_full_name);
  if (iter_variable_full_name != transform_cache.end()) {
    auto iter_transform_result = iter_variable_full_name->second.find(name());
    if (iter_transform_result != iter_variable_full_name->second.end()) {
      // The transformation has been evaluated before.
      data.variant_ = iter_transform_result->second.variant_;
      SRSECURITY_LOG_TRACE("transform cache hit: {} {}", variable_full_name.tostring(), name());
      return true;
    }
  } else {
    iter_variable_full_name =
        transform_cache
            .emplace(variable_full_name,
                     std::unordered_map<const char*, Common::EvaluateResults::Element>{})
            .first;
  }

  // Evaluate the transformation and store the result in the cache
  auto iter_transform_result =
      iter_variable_full_name->second.emplace(name(), Common::EvaluateResults::Element()).first;
  Common::EvaluateResults::Element& result = iter_transform_result->second;
  result.string_buffer_ = evaluate(std::get<std::string_view>(data.variant_));
  if (!result.string_buffer_.empty()) [[likely]] {
    result.variant_ = result.string_buffer_;
    data.variant_ = result.variant_;
    return true;
  }

  return false;
}
} // namespace Transformation
} // namespace SrSecurity
