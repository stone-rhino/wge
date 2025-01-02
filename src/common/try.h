#pragma once

#define TRY_NOCATCH(expr)                                                                          \
  do {                                                                                             \
    try {                                                                                          \
      expr;                                                                                        \
    } catch (...) {                                                                                \
    }                                                                                              \
  } while (0);
