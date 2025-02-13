#pragma once

#define RULE_TEST(id)                                                                              \
  void rule##id##Test(const SrSecurity::Rule& rule);                                               \
  class Rule##id##InitHelper {                                                                     \
  public:                                                                                          \
    Rule##id##InitHelper() {                                                                       \
      CrsTest::rule_tests_.emplace(id, std::bind(rule##id##Test, std::placeholders::_1));            \
    }                                                                                              \
  } rule_##id##init_helper;                                                                        \
  void rule##id##Test(const SrSecurity::Rule& rule)
