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
