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
#include "transformation/lowercase.h"
#include "variable/tx.h"

namespace SrSecurity {
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

  Common::EvaluateResults::Element transform_buffer(data, "");
  Variable::Tx variable(std::string("test"), std::nullopt, false, false);
  bool ret = trans->evaluate(*t_, &variable, transform_buffer);
  EXPECT_TRUE(ret);
  std::string_view result = std::get<std::string_view>(transform_buffer.variant_);
  EXPECT_EQ(result, "hello, world!hello, world!hello, world!hello, world!hello, world!");

  Common::EvaluateResults::Element transform_buffe2(data, "");
  ret = trans->evaluate(*t_, &variable, transform_buffe2);
  EXPECT_TRUE(ret);
  std::string_view result2 = std::get<std::string_view>(transform_buffe2.variant_);
  EXPECT_EQ(result.data(), result2.data());
}

TEST_F(CacheTest, notHitWithDifferentVaraible) {
  std::unique_ptr<Transformation::TransformBase> trans = std::make_unique<LowerCase>();
  Common::Variant data = "Hello, World!";

  Common::EvaluateResults::Element transform_buffer(data, "");
  Variable::Tx variable(std::string("test"), std::nullopt, false, false);
  bool ret = trans->evaluate(*t_, &variable, transform_buffer);
  EXPECT_TRUE(ret);
  std::string_view result = std::get<std::string_view>(transform_buffer.variant_);
  EXPECT_EQ(result, "hello, world!");

  Common::EvaluateResults::Element transform_buffe2(data, "");
  Variable::Tx variable2(std::string("test2"), std::nullopt, false, false);
  ret = trans->evaluate(*t_, &variable2, transform_buffe2);
  EXPECT_TRUE(ret);
  std::string_view result2 = std::get<std::string_view>(transform_buffe2.variant_);
  EXPECT_EQ(result, result2);
  EXPECT_NE(result.data(), result2.data());
}

TEST_F(CacheTest, notHitWithLessThanThreshold) {
  std::unique_ptr<Transformation::TransformBase> trans = std::make_unique<LowerCase>();
  std::string_view test_data = "Hello, World!";
  Common::Variant data = test_data;

  Common::EvaluateResults::Element transform_buffer(data, "");
  Variable::Tx variable(std::string("test"), std::nullopt, false, false);
  bool ret = trans->evaluate(*t_, &variable, transform_buffer);
  EXPECT_TRUE(ret);
  std::string_view result = std::get<std::string_view>(transform_buffer.variant_);
  EXPECT_EQ(result, "hello, world!");

  Common::EvaluateResults::Element transform_buffe2(data, "");
  ret = trans->evaluate(*t_, &variable, transform_buffe2);
  EXPECT_TRUE(ret);
  std::string_view result2 = std::get<std::string_view>(transform_buffe2.variant_);
  EXPECT_NE(result.data(), result2.data());
}
} // namespace Transformation
} // namespace SrSecurity