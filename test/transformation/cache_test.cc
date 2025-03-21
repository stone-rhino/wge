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
  Common::Variant data = "Hello, World!";

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
  EXPECT_EQ(result.data(), result2.data());
}

TEST_F(CacheTest, notHit) {
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
} // namespace Transformation
} // namespace SrSecurity