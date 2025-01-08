#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "antlr4/parser.h"

namespace SrSecurity {
namespace Parser {
class AuditLogConfigTest : public testing::Test {};

TEST_F(AuditLogConfigTest, AuditEngine) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditEngine On)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_, Antlr4::Parser::AuditLogConfig::AuditEngine::On);

  directive = R"(SecAuditEngine Off)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_,
            Antlr4::Parser::AuditLogConfig::AuditEngine::Off);

  directive = R"(SecAuditEngine RelevantOnly)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_,
            Antlr4::Parser::AuditLogConfig::AuditEngine::RelevantOnly);
}

TEST_F(AuditLogConfigTest, AuditLog) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLog /root/foo/bar.log)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().log_path_, "/root/foo/bar.log");
}

TEST_F(AuditLogConfigTest, AuditLog2) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLog2 /root/foo/bar.log)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().log_path2_, "/root/foo/bar.log");
}

TEST_F(AuditLogConfigTest, AuditLogDirMode) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogDirMode 0755)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  int mode = ::strtol("0755", nullptr, 8);
  EXPECT_EQ(parser.auditLogConfig().dir_mode_, mode);
}

TEST_F(AuditLogConfigTest, AuditLogFormat) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogFormat JSON)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().format_, Antlr4::Parser::AuditLogConfig::AuditFormat::Json);

  directive = R"(SecAuditLogFormat Native)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().format_, Antlr4::Parser::AuditLogConfig::AuditFormat::Native);
}

TEST_F(AuditLogConfigTest, AuditLogFileMode) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogFileMode 0755)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  int mode = ::strtol("0755", nullptr, 8);
  EXPECT_EQ(parser.auditLogConfig().file_mode_, mode);
}

TEST_F(AuditLogConfigTest, AuditLogParts) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogParts ABCDEFGHIJKZ)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  for (size_t i = 0; i < static_cast<size_t>(Antlr4::Parser::AuditLogConfig::AuditLogPart::End);
       i++) {
    EXPECT_TRUE(parser.auditLogConfig().log_parts_[i]);
  }

  directive = R"(SecAuditLogParts Hello)";
  error = parser.load(directive);
  EXPECT_FALSE(error.empty());
}

TEST_F(AuditLogConfigTest, AuditLogRelevantStatus) {
  Antlr4::Parser parser;

  std::string directive = R"EOF(SecAuditLogRelevantStatus "^(?:5|4(?!04))")EOF";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().relevant_status_regex_, "^(?:5|4(?!04))");

  directive = R"(SecAuditLogRelevantStatus Hello)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().relevant_status_regex_, "Hello");
}

TEST_F(AuditLogConfigTest, AuditLogStorageDir) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogStorageDir "/foo/bar")";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().storage_dir_, "/foo/bar");
}

TEST_F(AuditLogConfigTest, AuditLogType) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogType Serial)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_,
            Antlr4::Parser::AuditLogConfig::AuditLogType::Serial);

  directive = R"(SecAuditLogType Concurrent)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_,
            Antlr4::Parser::AuditLogConfig::AuditLogType::Concurrent);

  directive = R"(SecAuditLogType HTTPS)";
  error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_,
            Antlr4::Parser::AuditLogConfig::AuditLogType::Https);

  directive = R"(SecAuditLogType asdf)";
  error = parser.load(directive);
  EXPECT_FALSE(error.empty());
}

TEST_F(AuditLogConfigTest, ComponentSignature) {
  Antlr4::Parser parser;

  std::string directive = R"(SecComponentSignature Hello)";
  std::string error = parser.load(directive);
  ASSERT_TRUE(error.empty());
  EXPECT_EQ(parser.auditLogConfig().component_signature_, "Hello");
}
} // namespace Parser
} // namespace SrSecurity