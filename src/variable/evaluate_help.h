#pragma once

#define RETURN_IF_COUNTER(value_container, value_finder)                                           \
  if (is_counter_) [[unlikely]] {                                                                  \
    if (sub_name_.empty()) {                                                                       \
      result.append(static_cast<int>(value_container.size()));                                     \
    } else {                                                                                       \
      auto iter = value_finder.find(sub_name_);                                                    \
      result.append(iter != value_finder.end() ? 1 : 0);                                           \
    }                                                                                              \
    return;                                                                                        \
  }

#define RETURN_VALUE_FIRST(value_container, value_finder)                                          \
  if (sub_name_.empty()) [[unlikely]] {                                                            \
    for (auto& elem : value_container) {                                                           \
      if (!hasExceptVariable(elem.first)) [[likely]] {                                             \
        result.append(elem.first);                                                                 \
      }                                                                                            \
    }                                                                                              \
  } else {                                                                                         \
    if (isRegex()) {                                                                               \
      for (auto& elem : value_container) {                                                         \
        if (!hasExceptVariable(elem.first)) [[likely]] {                                           \
          if (match(elem.first)) {                                                                 \
            result.append(elem.first);                                                             \
          }                                                                                        \
        }                                                                                          \
      }                                                                                            \
    } else {                                                                                       \
      if (!hasExceptVariable(sub_name_)) [[likely]] {                                              \
        auto iter = value_finder.find(sub_name_);                                                  \
        if (iter != value_finder.end()) {                                                          \
          result.append(iter->first);                                                              \
        }                                                                                          \
      }                                                                                            \
    }                                                                                              \
  }

#define RETURN_VALUE_SECOND(value_container, value_finder)                                         \
  if (sub_name_.empty()) [[unlikely]] {                                                            \
    for (auto& elem : value_container) {                                                           \
      if (!hasExceptVariable(elem.first)) [[likely]] {                                             \
        result.append(elem.second);                                                                \
      }                                                                                            \
    }                                                                                              \
  } else {                                                                                         \
    if (isRegex()) {                                                                               \
      for (auto& elem : value_container) {                                                         \
        if (!hasExceptVariable(elem.first)) [[likely]] {                                           \
          if (match(elem.first)) {                                                                 \
            result.append(elem.second);                                                            \
          }                                                                                        \
        }                                                                                          \
      }                                                                                            \
    } else {                                                                                       \
      if (!hasExceptVariable(sub_name_)) [[likely]] {                                              \
        auto iter = value_finder.find(sub_name_);                                                  \
        if (iter != value_finder.end()) {                                                          \
          result.append(iter->second);                                                             \
        }                                                                                          \
      }                                                                                            \
    }                                                                                              \
  }
