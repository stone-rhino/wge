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

#include "engine.h"
#include "transformation/html_entity_decode.h"
#include "transformation/lowercase.h"
#include "transformation/url_decode.h"
#include "variable/args.h"
#include "variable/tx.h"

namespace Wge {
namespace Transformation {
class CacheTest : public ::testing::Test {
protected:
  void SetUp() override {
    engine_.init();
    t_ = engine_.makeTransaction();
  }

protected:
  Engine engine_;
  std::unique_ptr<Transaction> t_;
};

TEST_F(CacheTest, hit) {
  std::unique_ptr<Transformation::TransformBase> trans = std::make_unique<LowerCase>();
  std::string_view test_data = "Hello, World!Hello, World!Hello, World!Hello, World!Hello, World!";
  Common::Variant data = test_data;

  Common::EvaluateElement transform_buffer(data, "");
  Variable::Tx variable(std::string("test"), std::nullopt, false, false, "");
  bool ret = trans->evaluate(*t_, &variable, transform_buffer, transform_buffer);
  EXPECT_TRUE(ret);
  std::string_view result = std::get<std::string_view>(transform_buffer.variant_);
  EXPECT_EQ(result, "hello, world!hello, world!hello, world!hello, world!hello, world!");

  Common::EvaluateElement transform_buffe2(data, "");
  ret = trans->evaluate(*t_, &variable, transform_buffe2, transform_buffe2);
  EXPECT_TRUE(ret);
  std::string_view result2 = std::get<std::string_view>(transform_buffe2.variant_);
  EXPECT_EQ(result.data(), result2.data());
}

// Issues #24
TEST_F(CacheTest, getDuplicateArgsCache) {
  std::unique_ptr<Transformation::TransformBase> url_trans = std::make_unique<UrlDecode>();

  // The first_test_data is ' aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa' after being urlencoded.
  std::string_view first_origin = " aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  std::string_view first_test_data = "%20aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  Common::Variant first_data = first_test_data;
  Common::EvaluateElement first_transform_buffer{first_data, ""};
  Variable::Args first_variable(std::string("test"), false, false, "");

  bool ret =
      url_trans->evaluate(*t_, &first_variable, first_transform_buffer, first_transform_buffer);
  EXPECT_TRUE(ret);
  EXPECT_EQ(std::get<std::string_view>(first_transform_buffer.variant_), first_origin);

  // The second_test_data is ' bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb' after being urlencoded.
  std::string_view second_origin = " bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  std::string_view second_test_data = "%20bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  Common::Variant second_data = second_test_data;
  Common::EvaluateElement second_transform_buffer{second_data, ""};
  Variable::Args second_variable(std::string("test"), false, false, "");

  ret =
      url_trans->evaluate(*t_, &second_variable, second_transform_buffer, second_transform_buffer);
  EXPECT_TRUE(ret);
  EXPECT_EQ(std::get<std::string_view>(second_transform_buffer.variant_), second_origin);
}

// Issues #24
TEST_F(CacheTest, executeDuplicateTransformation) {
  std::vector<std::unique_ptr<Transformation::TransformBase>> trans;
  trans.push_back(std::make_unique<UrlDecode>());
  trans.push_back(std::make_unique<UrlDecode>());

  // The test_data is the result of URL-encoding the string '<><><><><><>' twice.
  std::string origin_data = "<><><><><><>";
  std::string_view test_data = "%253c%253e%253c%253e%253c%253e%253c%253e%253c%253e%253c%253e";

  Common::Variant data = test_data;
  Common::EvaluateElement transform_buffer{data, ""};
  Variable::Args variable(std::string("test"), false, false, "");
  bool ret;
  for (const auto& transformation : trans) {
    ret = transformation->evaluate(*t_, &variable, transform_buffer, transform_buffer);
  }
  EXPECT_TRUE(ret);
  EXPECT_EQ(std::get<std::string_view>(transform_buffer.variant_), origin_data);
}

// Issues #24
TEST_F(CacheTest, duplicateArgsDifferentTrans) {
  std::vector<std::unique_ptr<Transformation::TransformBase>> trans1;
  trans1.push_back(std::make_unique<UrlDecode>());
  trans1.push_back(std::make_unique<HtmlEntityDecode>());

  std::vector<std::unique_ptr<Transformation::TransformBase>> trans2;
  trans2.push_back(std::make_unique<HtmlEntityDecode>());

  // The test_data is the HTML entity encoded and URL encoded form of '<><><><><>'.
  std::string_view test_data =
      "%26lt%3b%26gt%3b%26lt%3b%26gt%3b%26lt%3b%26gt%3b%26lt%3b%26gt%3b%26lt%3b%26gt%3b";

  Common::Variant data1 = test_data;
  Common::EvaluateElement transform_buffer1{data1, ""};
  Variable::Args variable(std::string("test"), false, false, "");
  bool ret1;
  for (const auto& tran : trans1) {
    ret1 = tran->evaluate(*t_, &variable, transform_buffer1, transform_buffer1);
  }
  EXPECT_TRUE(ret1);

  Common::Variant data2 = test_data;
  Common::EvaluateElement transform_buffer2{data2, ""};
  bool ret2;
  for (const auto& tran : trans2) {
    ret2 = tran->evaluate(*t_, &variable, transform_buffer2, transform_buffer2);
  }
  EXPECT_FALSE(ret2);
  EXPECT_NE(std::get<std::string_view>(transform_buffer1.variant_),
            std::get<std::string_view>(transform_buffer2.variant_));
}
} // namespace Transformation
} // namespace Wge