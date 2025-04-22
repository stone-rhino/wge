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
#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"
#include "engine.h"

namespace Wge {
namespace Parsr {
class EngineConfigTest : public testing::Test {
private:
  // Use for specific the main thread id, so that the ASSERT_IS_MAIN_THREAD macro can work
  // correctly in the test.
  Engine main_thread_id_init_helper_;
};

TEST_F(EngineConfigTest, EngineConfig) {
  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess On
  SecResponseBodyAccess On
  SecRuleEngine On
  SecTmpSaveUploadedFiles On
  SecUploadKeepFiles On
  SecXmlExternalEntity On
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_response_body_access_, EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_rule_engine_, EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_upload_keep_files_, EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_xml_external_entity_, EngineConfig::Option::On);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess Off
  SecResponseBodyAccess Off
  SecRuleEngine Off
  SecTmpSaveUploadedFiles Off
  SecUploadKeepFiles Off
  SecXmlExternalEntity Off
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, EngineConfig::Option::Off);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess Off
  SecResponseBodyAccess Off
  SecRuleEngine DetectionOnly
  SecTmpSaveUploadedFiles Off
  SecUploadKeepFiles Off
  SecXmlExternalEntity Off
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(result.has_value());

    const auto& engine_config = parser.engineConfig();
    ASSERT_EQ(engine_config.is_request_body_access_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, EngineConfig::Option::DetectionOnly);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, EngineConfig::Option::Off);
  }

  {
    const std::string directive = R"(# Test engine config
  SecRequestBodyAccess DetectionOnly
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(!result.has_value());
  }

  {
    const std::string directive = R"(# Test engine config
  SecResponseBodyAccess DetectionOnly
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(!result.has_value());
  }

  {
    const std::string directive = R"(# Test engine config
  SecTmpSaveUploadedFiles DetectionOnly
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(!result.has_value());
  }

  {
    const std::string directive = R"(# Test engine config
  SecUploadKeepFiles DetectionOnly
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(!result.has_value());
  }

  {
    const std::string directive = R"(# Test engine config
  SecXmlExternalEntity DetectionOnly
  )";

    Antlr4::Parser parser;
    auto result = parser.load(directive);
    ASSERT_TRUE(!result.has_value());
  }
}

TEST_F(EngineConfigTest, ResponseBodyMimeType) {
  const std::string directive = R"(# Test engine config
  SecResponseBodyMimeType text/html text/plain
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  ASSERT_EQ(engine_config.response_body_mime_types_.size(), 2);
  ASSERT_EQ(engine_config.response_body_mime_types_[0], "text/html");
  ASSERT_EQ(engine_config.response_body_mime_types_[1], "text/plain");
}

TEST_F(EngineConfigTest, ResponseBodyMimeTypeClear) {
  const std::string directive = R"(# Test engine config
  SecResponseBodyMimeType text/html text/plain
  SecResponseBodyMimeTypesClear
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  ASSERT_EQ(engine_config.response_body_mime_types_.size(), 0);
}

TEST_F(EngineConfigTest, RequestBodyLimit) {
  const std::string directive = R"(# Test engine config
  SecRequestBodyLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.request_body_limit_, 1024);
}

TEST_F(EngineConfigTest, RequestBodyNoFilesLimit) {
  const std::string directive = R"(# Test engine config
  SecRequestBodyNoFilesLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.request_body_no_files_limit_, 1024);
}

TEST_F(EngineConfigTest, RequestBodyJsonDepthLimit) {
  const std::string directive = R"(# Test engine config
  SecRequestBodyJsonDepthLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.request_body_json_depth_limit_, 1024);
}

TEST_F(EngineConfigTest, RequestBodyLimitAction) {
  const std::string directive = R"(# Test engine config
  SecRequestBodyLimitAction Reject
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.request_body_limit_action_, EngineConfig::BodyLimitAction::Reject);
}

TEST_F(EngineConfigTest, ResponseBodyLimit) {
  const std::string directive = R"(# Test engine config
  SecResponseBodyLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.response_body_limit_, 1024);
}

TEST_F(EngineConfigTest, ResponseBodyLimitAction) {
  const std::string directive = R"(# Test engine config
  SecResponseBodyLimitAction Reject
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.response_body_limit_action_, EngineConfig::BodyLimitAction::Reject);
}

TEST_F(EngineConfigTest, ArgumentsLimit) {
  const std::string directive = R"(# Test engine config
  SecArgumentsLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.arguments_limit_, 1024);
}

TEST_F(EngineConfigTest, ArgumentSeparator) {
  const std::string directive = R"(# Test engine config
  SecArgumentSeparator !
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.argument_separator_, '!');
}

TEST_F(EngineConfigTest, UnicodeMapFile) {
  const std::string directive = R"(# Test engine config
  SecUnicodeMapFile /aaa/bbb 123456
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.unicode_map_file_, "/aaa/bbb");
  EXPECT_EQ(engine_config.unicode_code_point_, 123456);
}

TEST_F(EngineConfigTest, PcreMatchLimit) {
  const std::string directive = R"(# Test engine config
  SecPcreMatchLimit 1024
  )";

  Antlr4::Parser parser;
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());

  const auto& engine_config = parser.engineConfig();
  EXPECT_EQ(engine_config.pcre_match_limit_, 1024);
}
} // namespace Parsr
} // namespace Wge