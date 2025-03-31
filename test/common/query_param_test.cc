#include <gtest/gtest.h>

#include "common/ragel/query_param.h"

TEST(Common, queryParam) {
  {
    SrSecurity::Common::Ragel::QueryParam query_param;
    query_param.init("a=1&b=2&c=3");
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map["a"], "1");
    EXPECT_EQ(map["b"], "2");
    EXPECT_EQ(map["c"], "3");

    auto linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0]->first, "a");
    EXPECT_EQ(linked[0]->second, "1");
    EXPECT_EQ(linked[1]->first, "b");
    EXPECT_EQ(linked[1]->second, "2");
    EXPECT_EQ(linked[2]->first, "c");
    EXPECT_EQ(linked[2]->second, "3");
  }

  {
    SrSecurity::Common::Ragel::QueryParam query_param;
    query_param.init("a=1&b&c=3");
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map["a"], "1");
    EXPECT_EQ(map["b"], "");
    EXPECT_EQ(map["c"], "3");

    auto linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0]->first, "a");
    EXPECT_EQ(linked[0]->second, "1");
    EXPECT_EQ(linked[1]->first, "b");
    EXPECT_EQ(linked[1]->second, "");
    EXPECT_EQ(linked[2]->first, "c");
    EXPECT_EQ(linked[2]->second, "3");
  }

  {
    SrSecurity::Common::Ragel::QueryParam query_param;
    query_param.init("a=&b&c=3");
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 0);

    auto linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 0);
  }
}