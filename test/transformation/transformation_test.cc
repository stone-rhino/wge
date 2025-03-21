#include <gtest/gtest.h>

#include "transformation/transform_include.h"

namespace SrSecurity {
namespace Transformation {
class TransformationTest : public ::testing::Test {};

TEST_F(TransformationTest, base64DecodeExt) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, base64Decode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, base64Encode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, cmdLine) {
  CmdLine cmdLine;

  // deleting all backslashes [\]
  {
    std::string data = R"(\test\ \data\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test data");
  }

  // deleting all double quotes ["]
  {
    std::string data = R"(\"test\ \"data\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test data");
  }

  // deleting all single quotes [']
  {
    std::string data = R"(\"test'\ \"data'\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test data");
  }

  // deleting all carets [^]
  {
    std::string data = R"(\"te^st'\ \"da^ta'\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test data");
  }

  // deleting spaces before a slash /
  {
    std::string data = R"(\"te^st'\           /\"da^ta'\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test/data");
  }

  // deleting spaces before an open parentesis [(]
  {
    std::string data = R"(\"te^st'\           /          (\"da^ta'\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test/(data");
  }

  // replacing all commas [,] and semicolon [;] into a space
  {
    std::string data = R"(\"te^st'\           /          (,\"da^t;a'\)";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test/( dat a");
  }

  // replacing all multiple spaces (including tab, newline, etc.) into one space
  {
    std::string data = "\\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a'\\";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test/( dat a");
  }

  // transform all characters to lowercase
  {
    std::string data = "\\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a_HELLO'\\";
    std::string result = cmdLine.evaluate(data);
    EXPECT_EQ(result, "test/( dat a_hello");
  }
}

TEST_F(TransformationTest, compressWhiteSpace) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, cssDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, escapeSeqDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, hexDecode) {
  HexDecode hexDecode;
  std::string data = "5468697320697320612074657374";
  std::string result = hexDecode.evaluate(data);
  EXPECT_EQ(result, "This is a test");
}

TEST_F(TransformationTest, hexEncode) {
  HexEncode hexEncode;
  std::string data = "This is a test";
  std::string result = hexEncode.evaluate(data);
  EXPECT_EQ(result, "5468697320697320612074657374");
}

TEST_F(TransformationTest, htmlEntityDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, jsDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, length) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, lowercase) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, md5) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalisePathWin) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalisePath) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalizePathWin) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalizePath) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, parityEven7Bit) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, parityOdd7Bit) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, ParityZero7Bit) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, removeComments) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, removeCommentChar) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, removeNulls) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, removeWhitespace) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, replaceComments) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, replaceNulls) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, sha1) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, sqlHexDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, trimLeft) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, trimRight) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, trim) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, upperCase) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, urlDecodeUni) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, urlDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, urlEncode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, utf8ToUnicode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}
} // namespace Transformation
} // namespace SrSecurity