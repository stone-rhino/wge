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
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_, AuditLogConfig::AuditEngine::On);

  directive = R"(SecAuditEngine Off)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_, AuditLogConfig::AuditEngine::Off);

  directive = R"(SecAuditEngine RelevantOnly)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_engine_, AuditLogConfig::AuditEngine::RelevantOnly);
}

TEST_F(AuditLogConfigTest, AuditLog) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLog /root/foo/bar1.log
  SecAuditLog /root/foo/bar2.log)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().log_path_, "/root/foo/bar2.log");
}

TEST_F(AuditLogConfigTest, AuditLog2) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLog2 /root/foo/bar1.log
  SecAuditLog2 /root/foo/bar2.log)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().log_path2_, "/root/foo/bar2.log");
}

TEST_F(AuditLogConfigTest, AuditLogDirMode) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogDirMode 0755)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  int mode = ::strtol("0755", nullptr, 8);
  EXPECT_EQ(parser.auditLogConfig().dir_mode_, mode);
}

TEST_F(AuditLogConfigTest, AuditLogFormat) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogFormat JSON)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().format_, AuditLogConfig::AuditFormat::Json);

  directive = R"(SecAuditLogFormat Native)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().format_, AuditLogConfig::AuditFormat::Native);
}

TEST_F(AuditLogConfigTest, AuditLogFileMode) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogFileMode 0755)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  int mode = ::strtol("0755", nullptr, 8);
  EXPECT_EQ(parser.auditLogConfig().file_mode_, mode);
}

TEST_F(AuditLogConfigTest, AuditLogParts) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogParts ABCDEFGHIJKZ)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  for (size_t i = 0; i < static_cast<size_t>(AuditLogConfig::AuditLogPart::End); i++) {
    EXPECT_TRUE(parser.auditLogConfig().log_parts_[i]);
  }

  directive = R"(SecAuditLogParts Hello)";
  result = parser.load(directive);
  ASSERT_TRUE(!result.has_value());
}

TEST_F(AuditLogConfigTest, AuditLogRelevantStatus) {
  Antlr4::Parser parser;

  std::string directive = R"EOF(SecAuditLogRelevantStatus "^(?:5|4(?!04))")EOF";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().relevant_status_regex_, "^(?:5|4(?!04))");

  directive = R"(SecAuditLogRelevantStatus Hello)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().relevant_status_regex_, "Hello");
}

TEST_F(AuditLogConfigTest, AuditLogStorageDir) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogStorageDir "/foo/bar")";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().storage_dir_, "/foo/bar");
}

TEST_F(AuditLogConfigTest, AuditLogType) {
  Antlr4::Parser parser;

  std::string directive = R"(SecAuditLogType Serial)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_, AuditLogConfig::AuditLogType::Serial);

  directive = R"(SecAuditLogType Concurrent)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_, AuditLogConfig::AuditLogType::Concurrent);

  directive = R"(SecAuditLogType HTTPS)";
  result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().audit_log_type_, AuditLogConfig::AuditLogType::Https);

  directive = R"(SecAuditLogType asdf)";
  result = parser.load(directive);
  ASSERT_TRUE(!result.has_value());
}

TEST_F(AuditLogConfigTest, ComponentSignature) {
  Antlr4::Parser parser;

  std::string directive = R"(SecComponentSignature Hello)";
  auto result = parser.load(directive);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(parser.auditLogConfig().component_signature_, "Hello");
}
} // namespace Parser
} // namespace SrSecurity