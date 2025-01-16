#include <string>

#include <gtest/gtest.h>

#include "antlr4/parser.h"

namespace SrSecurity {
namespace Parsr {
class EngineConfigTest : public testing::Test {};

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
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::On);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::On);
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
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::Off);
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
    ASSERT_EQ(engine_config.is_request_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_response_body_access_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_rule_engine_, Antlr4::Parser::EngineConfig::Option::DetectionOnly);
    ASSERT_EQ(engine_config.is_tmp_save_uploaded_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_upload_keep_files_, Antlr4::Parser::EngineConfig::Option::Off);
    ASSERT_EQ(engine_config.is_xml_external_entity_, Antlr4::Parser::EngineConfig::Option::Off);
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
} // namespace Parsr
} // namespace SrSecurity