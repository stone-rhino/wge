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
#include <gtest/gtest.h>

#include "common/ragel/query_param.h"

TEST(Common, queryParam) {
  std::forward_list<std::string> buffer;
  {
    Wge::Common::Ragel::QueryParam query_param;
    query_param.init("a=1&b=2&c=3", buffer);
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.find("a")->second, "1");
    EXPECT_EQ(map.find("b")->second, "2");
    EXPECT_EQ(map.find("c")->second, "3");

    auto& linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0].first, "a");
    EXPECT_EQ(linked[0].second, "1");
    EXPECT_EQ(linked[1].first, "b");
    EXPECT_EQ(linked[1].second, "2");
    EXPECT_EQ(linked[2].first, "c");
    EXPECT_EQ(linked[2].second, "3");
  }

  {
    Wge::Common::Ragel::QueryParam query_param;
    query_param.init("a=1&b&c=3", buffer);
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.find("a")->second, "1");
    EXPECT_EQ(map.find("b")->second, "");
    EXPECT_EQ(map.find("c")->second, "3");

    auto& linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0].first, "a");
    EXPECT_EQ(linked[0].second, "1");
    EXPECT_EQ(linked[1].first, "b");
    EXPECT_EQ(linked[1].second, "");
    EXPECT_EQ(linked[2].first, "c");
    EXPECT_EQ(linked[2].second, "3");
  }

  {
    Wge::Common::Ragel::QueryParam query_param;
    query_param.init("a=&b&c=3", buffer);
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);

    auto linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
  }

  {
    Wge::Common::Ragel::QueryParam query_param;
    query_param.init(
        "a=Can+I+please+have+a+Python+tutorial+for+qr+code+scanning%3f&b=2&%3b%2721io)=3", buffer);
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.find("a")->second, "Can I please have a Python tutorial for qr code scanning?");
    EXPECT_EQ(map.find("b")->second, "2");
    EXPECT_EQ(map.find(";'21io)")->second, "3");

    auto linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0].first, "a");
    EXPECT_EQ(linked[0].second, "Can I please have a Python tutorial for qr code scanning?");
    EXPECT_EQ(linked[1].first, "b");
    EXPECT_EQ(linked[1].second, "2");
    EXPECT_EQ(linked[2].first, ";'21io)");
    EXPECT_EQ(linked[2].second, "3");
  }

  {
    Wge::Common::Ragel::QueryParam query_param;
    query_param.init("&&a=1&&=2&b==c=3&&", buffer);
    auto map = query_param.get();
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.find("a")->second, "1");
    EXPECT_EQ(map.find("")->second, "2");
    EXPECT_EQ(map.find("b")->second, "=c=3");

    auto& linked = query_param.getLinked();
    EXPECT_EQ(linked.size(), 3);
    EXPECT_EQ(linked[0].first, "a");
    EXPECT_EQ(linked[0].second, "1");
    EXPECT_EQ(linked[1].first, "");
    EXPECT_EQ(linked[1].second, "2");
    EXPECT_EQ(linked[2].first, "b");
    EXPECT_EQ(linked[2].second, "=c=3");
  }
}
