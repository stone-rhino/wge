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

#include "transaction_test.h"

namespace Wge {
bool isEqual(const Transaction::RequestLineInfo& a, const Transaction::RequestLineInfo& b) {
  return a.method_ == b.method_ && a.uri_raw_ == b.uri_raw_ && a.uri_ == b.uri_ &&
         a.relative_uri_ == b.relative_uri_ && a.query_ == b.query_ && a.protocol_ == b.protocol_ &&
         a.version_ == b.version_ && a.base_name_ == b.base_name_ &&
         a.uri_buffer_ == b.uri_buffer_ && a.relative_uri_buffer_ == b.relative_uri_buffer_ &&
         a.base_name_buffer_ == b.base_name_buffer_;
}

TEST_F(TransactionTest, ProcessUri) {
  {
    auto trans1 = engine_.makeTransaction();
    auto trans2 = engine_.makeTransaction();
    std::string uri = "/";
    std::string method = "GET";
    std::string version = "1.1";
    std::string request_line = method + " " + uri + " HTTP/" + version;
    trans1->processUri(uri, method, version);
    trans2->processUri(request_line);
    auto request_line_info = trans1->getRequestLineInfo();
    EXPECT_EQ(request_line_info.method_, method);
    EXPECT_EQ(request_line_info.uri_raw_, uri);
    EXPECT_EQ(request_line_info.uri_, "/");
    EXPECT_EQ(request_line_info.relative_uri_, "/");
    EXPECT_EQ(request_line_info.query_, "");
    EXPECT_EQ(request_line_info.protocol_, "HTTP/1.1");
    EXPECT_EQ(request_line_info.version_, version);
    EXPECT_EQ(request_line_info.base_name_, "");
    EXPECT_EQ(request_line_info.uri_buffer_, "");
    EXPECT_EQ(request_line_info.relative_uri_buffer_, "");
    EXPECT_EQ(request_line_info.base_name_buffer_, "");
    EXPECT_TRUE(isEqual(request_line_info, trans2->getRequestLineInfo()));
  }

  {
    auto trans1 = engine_.makeTransaction();
    auto trans2 = engine_.makeTransaction();
    std::string uri = "http://localhost:8080/path/";
    std::string method = "POST";
    std::string version = "1.1";
    std::string request_line = method + " " + uri + " HTTP/" + version;
    trans1->processUri(uri, method, version);
    trans2->processUri(request_line);
    auto request_line_info = trans1->getRequestLineInfo();
    EXPECT_EQ(request_line_info.method_, method);
    EXPECT_EQ(request_line_info.uri_raw_, uri);
    EXPECT_EQ(request_line_info.uri_, "/path/");
    EXPECT_EQ(request_line_info.relative_uri_, "http://localhost:8080/path/");
    EXPECT_EQ(request_line_info.query_, "");
    EXPECT_EQ(request_line_info.protocol_, "HTTP/1.1");
    EXPECT_EQ(request_line_info.version_, version);
    EXPECT_EQ(request_line_info.base_name_, "");
    EXPECT_EQ(request_line_info.uri_buffer_, "");
    EXPECT_EQ(request_line_info.relative_uri_buffer_, "");
    EXPECT_EQ(request_line_info.base_name_buffer_, "");
    EXPECT_TRUE(isEqual(request_line_info, trans2->getRequestLineInfo()));
  }

  {
    auto trans1 = engine_.makeTransaction();
    auto trans2 = engine_.makeTransaction();
    std::string uri = "/api/data#section2";
    std::string method = "GET";
    std::string version = "1.1";
    std::string request_line = method + " " + uri + " HTTP/" + version;
    trans1->processUri(uri, method, version);
    trans2->processUri(request_line);
    auto request_line_info = trans1->getRequestLineInfo();
    EXPECT_EQ(request_line_info.method_, method);
    EXPECT_EQ(request_line_info.uri_raw_, uri);
    EXPECT_EQ(request_line_info.uri_, "/api/data");
    EXPECT_EQ(request_line_info.relative_uri_, "/api/data");
    EXPECT_EQ(request_line_info.query_, "");
    EXPECT_EQ(request_line_info.protocol_, "HTTP/1.1");
    EXPECT_EQ(request_line_info.version_, version);
    EXPECT_EQ(request_line_info.base_name_, "data");
    EXPECT_EQ(request_line_info.uri_buffer_, "");
    EXPECT_EQ(request_line_info.relative_uri_buffer_, "");
    EXPECT_EQ(request_line_info.base_name_buffer_, "");
    EXPECT_TRUE(isEqual(request_line_info, trans2->getRequestLineInfo()));
  }

  {
    auto trans1 = engine_.makeTransaction();
    auto trans2 = engine_.makeTransaction();
    std::string uri = "https://example.com/path/index.php?id=1#top";
    std::string method = "GET";
    std::string version = "2.0";
    std::string request_line = method + " " + uri + " HTTP/" + version;
    trans1->processUri(uri, method, version);
    trans2->processUri(request_line);
    auto request_line_info = trans1->getRequestLineInfo();
    EXPECT_EQ(request_line_info.method_, method);
    EXPECT_EQ(request_line_info.uri_raw_, uri);
    EXPECT_EQ(request_line_info.uri_, "/path/index.php?id=1");
    EXPECT_EQ(request_line_info.relative_uri_, "https://example.com/path/index.php");
    EXPECT_EQ(request_line_info.query_, "id=1");
    EXPECT_EQ(request_line_info.protocol_, "HTTP/2.0");
    EXPECT_EQ(request_line_info.version_, version);
    EXPECT_EQ(request_line_info.base_name_, "index.php");
    EXPECT_EQ(request_line_info.uri_buffer_, "");
    EXPECT_EQ(request_line_info.relative_uri_buffer_, "");
    EXPECT_EQ(request_line_info.base_name_buffer_, "");
    EXPECT_TRUE(isEqual(request_line_info, trans2->getRequestLineInfo()));
  }

  {
    auto trans1 = engine_.makeTransaction();
    auto trans2 = engine_.makeTransaction();
    std::string uri =
        "http://example.com/pa%3b%3f+th/ind%3b%3f+ex.php?i%3b%3f+d=1%3b%3f+1#t%3b%3f+op";
    std::string method = "POST";
    std::string version = "1.1";
    std::string request_line = method + " " + uri + " HTTP/" + version;
    trans1->processUri(uri, method, version);
    trans2->processUri(request_line);
    auto request_line_info = trans1->getRequestLineInfo();
    EXPECT_EQ(request_line_info.method_, method);
    EXPECT_EQ(request_line_info.uri_raw_, uri);
    EXPECT_EQ(request_line_info.uri_, "/pa;?+th/ind;?+ex.php?i;?+d=1;?+1");
    EXPECT_EQ(request_line_info.relative_uri_, "http://example.com/pa;?+th/ind;?+ex.php");
    EXPECT_EQ(request_line_info.query_, "i%3b%3f+d=1%3b%3f+1");
    EXPECT_EQ(request_line_info.protocol_, "HTTP/1.1");
    EXPECT_EQ(request_line_info.version_, version);
    EXPECT_EQ(request_line_info.base_name_, "ind;?+ex.php");
    EXPECT_EQ(request_line_info.uri_buffer_, "/pa;?+th/ind;?+ex.php?i;?+d=1;?+1");
    EXPECT_EQ(request_line_info.relative_uri_buffer_, "http://example.com/pa;?+th/ind;?+ex.php");
    EXPECT_EQ(request_line_info.base_name_buffer_, "ind;?+ex.php");
    EXPECT_TRUE(isEqual(request_line_info, trans2->getRequestLineInfo()));
  }
}
} // namespace Wge