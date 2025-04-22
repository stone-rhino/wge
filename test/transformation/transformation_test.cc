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

#include "transformation/transform_include.h"

#include "../../src/common/duration.h"

namespace SrSecurity {
namespace Transformation {
class TransformationTest : public ::testing::Test {};

TEST_F(TransformationTest, base64DecodeExt) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, base64Decode) {
  const Base64Decode base64_decode;

  std::string data = R"(This is a@ test)";
  std::string result;
  bool ret = base64_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = "VGhpcyBpcyBhIHRlc3Q";
  ret = base64_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = "VGhpcyBpcyBhIHRlc3=Q";
  ret = base64_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = "VGhpcyBpcyBhIHRlc===";
  ret = base64_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = "VGhpcyBpcyBhIHRlc3Q=";
  ret = base64_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");
}

TEST_F(TransformationTest, base64Encode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, cmdLine) {
  const CmdLine cmd_line;

  // Test that prescan is working, and that will not copy if there is no transformation
  {
    std::string data = R"(this is a test data)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_FALSE(ret);
    EXPECT_TRUE(result.empty());
  }

  // Test that prescan is working, and that will hold the token if there is a transformation
  {
    std::string data = R"(this        is a ;;;;;;;;;test data)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test data");
  }

  // deleting all backslashes [\]
  {
    std::string data = R"(this is a \test\ \data\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test data");
  }

  // deleting all double quotes ["]
  {
    std::string data = R"(this is a \"test\ \"data\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test data");
  }

  // deleting all single quotes [']
  {
    std::string data = R"(this is a \"test'\ \"data'\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test data");
  }

  // deleting all carets [^]
  {
    std::string data = R"(this is a \"te^st'\ \"da^ta'\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test data");
  }

  // deleting spaces before a slash /
  {
    std::string data = R"(this is a \"te^st'\           /\"da^ta'\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test/data");
  }

  // deleting spaces before an open parentesis [(]
  {
    std::string data = R"(this is a \"te^st'\           /          (\"da^ta'\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test/(data");
  }

  // replacing all commas [,] and semicolon [;] into a space
  {
    std::string data = R"(this is a \"te^st'\           /          (,\"da^t;a'\)";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test/( dat a");
  }

  // replacing all multiple spaces (including tab, newline, etc.) into one space
  {
    std::string data = "this is a \\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a'\\";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test/( dat a");
  }

  // transform all characters to lowercase
  {
    std::string data = "this is a \\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a_HELLO'\\";
    std::string result;
    bool ret = cmd_line.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "this is a test/( dat a_hello");
  }
}

TEST_F(TransformationTest, compressWhiteSpace) {
  const CompressWhiteSpace compress_white_space;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = compress_white_space.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This   is   a   test)";
  ret = compress_white_space.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = "This \f\t\n\r\v\xa0 is \f\t\n\r\v\xa0 a \f\t\n\r\v\xa0 test";
  ret = compress_white_space.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");
}

TEST_F(TransformationTest, cssDecode) {
  const CssDecode css_decode;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = css_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This\ is\ a\ test)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(T\hi\s is a test)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(This\ is\ a\ test\)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(This\ is\ a\ test\ \)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test ");

  data = R"(\1254\3468 is\ is\ a\ test\ \ \)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test  ");

  data = R"(\12354\123468is\ is\ a\ test\ \ \)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test  ");

  data = R"(\12354\123468\6is\ is\ a\ test\ \ \)";
  ret = css_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "Th\u0006is is a test  ");

  {
    char data[] = "Test\u0000Case";
    ret = css_decode.evaluate({data, sizeof(data) - 1}, result);
    EXPECT_FALSE(ret);
    EXPECT_TRUE(result.empty());
  }

  {
    // clang-format off
    char data[] = "test\\a\\b\\f\\n\\r\\t\\v\\?\\'\\\"\\\u0000\\12\\123\\1234\\12345\\123456\\ff01\\ff5e\\\n\\\u0000  string";
    ret = css_decode.evaluate({data, sizeof(data) - 1},result);
    EXPECT_TRUE(ret);
    char expect_data[] = "test\n\u000b\u000fnrtv?'\"\u0000\u0012#4EV!~\u0000  string";
    EXPECT_TRUE(memcmp(result.data(), expect_data, sizeof(expect_data) - 1) == 0);
    // clang-format on
  }

  {
    data = "\\1A\\1 A\\1234567\\123456 7\\1x\\1 x";
    ret = css_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "\u001a\u0001AV7V7\u0001x\u0001x");
  }
}

TEST_F(TransformationTest, escapeSeqDecode) {
  const EscapeSeqDecode escape_seq_decode;

  {
    std::string data = R"(This is a test data)";
    std::string result;
    bool ret = escape_seq_decode.evaluate(data, result);
    EXPECT_FALSE(ret);
    EXPECT_TRUE(result.empty());
  }

  {
    std::string data = R"(This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab \101 \01 \1)";
    std::string result;
    bool ret = escape_seq_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab A \1 \1");
  }
}

TEST_F(TransformationTest, hexDecode) {
  const HexDecode hex_decode;

  {
    std::string data = "G5468697320697320612074657374";
    std::string result;
    bool ret = hex_decode.evaluate(data, result);
    EXPECT_FALSE(ret);
    EXPECT_TRUE(result.empty());
  }

  {
    std::string data = "a";
    std::string result;
    bool ret = hex_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "\n");
  }

  {
    std::string data = "5468G697320697320612074657374";
    std::string result;
    bool ret = hex_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "Th");
  }

  {
    std::string data = "5468697320697320612074657374";
    std::string result;
    bool ret = hex_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "This is a test");
  }
}

TEST_F(TransformationTest, hexEncode) {
  HexEncode hexEncode;
  std::string data = "This is a test";
  std::string result;
  bool ret = hexEncode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "5468697320697320612074657374");
}

TEST_F(TransformationTest, htmlEntityDecode) {
  const HtmlEntityDecode html_entity_decode;

  // clang-format off
  std::vector<std::pair<std::string,std::string>> test_cases = {
    {"&#x54;&#x68;&#x69;&#x73;&#x20;&#x69;&#x73;&#x20;&#x61;&#x20;&#x74;&#x65;&#x73;&#x74;", "This is a test"},
    {"&#84;&#104;&#105;&#115;&#32;&#105;&#115;&#32;&#97;&#32;&#116;&#101;&#115;&#116;", "This is a test"},
    {"&#x54;his is a test", "This is a test"},
    {"&#84;his is a test", "This is a test"},
    {"&#x54;his is a test", "This is a test"},
    {"&#84;his is a test", "This is a test"},
    {"&amp; &lt; &gt; &quot; &apos; &nbsp;", "& < > \" '  "},
    {"&amp;&apos;this&apos;&nbsp;&quot;is&quot;&nbsp;a&nbsp;&lt;te&#115;&#116;&gt;", "&'this' \"is\" a <test>"}
  };
  // clang-format on

  // Test that prescan is working, and that will not copy if there is no transformation
  {
    std::string data = R"(This is a test data)";
    std::string result;
    bool ret = html_entity_decode.evaluate(data, result);
    EXPECT_FALSE(ret);
    EXPECT_TRUE(result.empty());
  }

  for (auto& test_case : test_cases) {
    std::string result;
    bool ret = html_entity_decode.evaluate(test_case.first, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, test_case.second);
  }

  // Test for not valid html entity
  {
    std::string data = "&amp; &lt; &gt; &quot; &apos; &nbsp; &notValid;";
    std::string result;
    bool ret = html_entity_decode.evaluate(data, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "& < > \" '   &notValid;");
  }
}

TEST_F(TransformationTest, jsDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, length) {
  const Length length;
  std::string data = R"(This is a test)";
  std::string result;
  bool ret = length.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "14");
  EXPECT_TRUE(length.convertToInt());
  int length_value = ::atoi(result.c_str());
  EXPECT_EQ(length_value, 14);
}

TEST_F(TransformationTest, lowercase) {
  const LowerCase lowercase;

  std::string data = R"(this is a test)";
  std::string result;
  bool ret = lowercase.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(THIS IS A TEST)";
  ret = lowercase.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "this is a test");

  data = R"(ThiS iS A TeSt~!@#$%^&*()_+)";
  ret = lowercase.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "this is a test~!@#$%^&*()_+");
}

TEST_F(TransformationTest, md5) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalisePathWin) {
  const NormalisePathWin normalise_path_win;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = normalise_path_win.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(\path\to\file)";
  ret = normalise_path_win.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "/path/to/file");

  // clang-format off
  std::vector<std::pair<std::string,std::string>> test_cases = {
    {".",""},
    {".\\",""},
    {".\\..", ".."},
    {".\\..\\", "../"},
    {"..", ".."},
    {"..\\", "../"},
    {"..\\.", ".."},
    {"..\\.\\", "../"},
    {"..\\..", "../.."},
    {"..\\..\\", "../../"},
    {"\\dir\\foo\\\\bar", "/dir/foo/bar"},
    {"dir\\foo\\\\bar\\", "dir/foo/bar/"},
    {"dir\\..\\foo", "foo"},
    {"dir\\..\\..\\foo", "../foo"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar", "../../foo/bar"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\.", "../../foo/bar"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\.\\", "../../foo/bar/"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\..", "../../foo"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\..\\", "../../foo/"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\", "../../foo/bar/"},
    {"dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar", "../../foo/bar"},
    {"dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar\\\\", "../../foo/bar/"},
    {"dir\\subdir\\subsubdir\\subsubsubdir\\..\\..\\..", "dir"},
    {"dir\\.\\subdir\\.\\subsubdir\\.\\subsubsubdir\\..\\..\\..", "dir"},
    {"dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..", "dir"},
    {"\\dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..\\", "/dir/"},
    {"\\.\\..\\.\\..\\..\\..\\..\\..\\..\\..\\\\u0000\\..\\etc\\.\\passwd", "/etc/passwd"},
  };
  // clang-format on

  for (size_t i = 0; i < test_cases.size(); ++i) {
    auto& test_case = test_cases[i];
    std::string result;
    bool ret = normalise_path_win.evaluate(test_case.first, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, test_case.second);
    if (!ret || result != test_case.second) {
      std::cout << "Test case " << i << " failed: " << test_case.first << " -> " << result
                << std::endl;
    }
  }
}

TEST_F(TransformationTest, normalisePath) {
  const NormalisePath normalise_path;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = normalise_path.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(/path/to/file)";
  ret = normalise_path.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  // clang-format off
  std::vector<std::pair<std::string,std::string>> test_cases = {
    {".",""},
    {"./",""},
    {"./..", ".."},
    {"./../", "../"},
    {"..", ".."},
    {"../", "../"},
    {"../.", ".."},
    {".././", "../"},
    {"../..", "../.."},
    {"../../", "../../"},
    {"/dir/foo//bar", "/dir/foo/bar"},
    {"dir/foo//bar/", "dir/foo/bar/"},
    {"dir/../foo", "foo"},
    {"dir/../../foo", "../foo"},
    {"dir/./.././../../foo/bar", "../../foo/bar"},
    {"dir/./.././../../foo/bar/.", "../../foo/bar"},
    {"dir/./.././../../foo/bar/./", "../../foo/bar/"},
    {"dir/./.././../../foo/bar/..", "../../foo"},
    {"dir/./.././../../foo/bar/../", "../../foo/"},
    {"dir/./.././../../foo/bar/", "../../foo/bar/"},
    {"dir//.//..//.//..//..//foo//bar", "../../foo/bar"},
    {"dir//.//..//.//..//..//foo//bar//", "../../foo/bar/"},
    {"dir/subdir/subsubdir/subsubsubdir/../../..", "dir"},
    {"dir/./subdir/./subsubdir/./subsubsubdir/../../..", "dir"},
    {"dir/./subdir/../subsubdir/../subsubsubdir/..", "dir"},
    {"/dir/./subdir/../subsubdir/../subsubsubdir/../", "/dir/"},
    {"/./.././../../../../../../../\\u0000/../etc/./passwd", "/etc/passwd"},
  };
  // clang-format on

  for (size_t i = 0; i < test_cases.size(); ++i) {
    auto& test_case = test_cases[i];
    std::string result;
    bool ret = normalise_path.evaluate(test_case.first, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, test_case.second);
    if (!ret || result != test_case.second) {
      std::cout << "Test case " << i << " failed: " << test_case.first << " -> " << result
                << std::endl;
    }
  }
}

TEST_F(TransformationTest, normalizePathWin) {
  const NormalizePathWin normalize_path_win;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = normalize_path_win.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(\path\to\file)";
  ret = normalize_path_win.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "/path/to/file");

  // clang-format off
  std::vector<std::pair<std::string,std::string>> test_cases = {
    {".",""},
    {".\\",""},
    {".\\..", ".."},
    {".\\..\\", "../"},
    {"..", ".."},
    {"..\\", "../"},
    {"..\\.", ".."},
    {"..\\.\\", "../"},
    {"..\\..", "../.."},
    {"..\\..\\", "../../"},
    {"\\dir\\foo\\\\bar", "/dir/foo/bar"},
    {"dir\\foo\\\\bar\\", "dir/foo/bar/"},
    {"dir\\..\\foo", "foo"},
    {"dir\\..\\..\\foo", "../foo"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar", "../../foo/bar"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\.", "../../foo/bar"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\.\\", "../../foo/bar/"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\..", "../../foo"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\..\\", "../../foo/"},
    {"dir\\.\\..\\.\\..\\..\\foo\\bar\\", "../../foo/bar/"},
    {"dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar", "../../foo/bar"},
    {"dir\\\\.\\\\..\\\\.\\\\..\\\\..\\\\foo\\\\bar\\\\", "../../foo/bar/"},
    {"dir\\subdir\\subsubdir\\subsubsubdir\\..\\..\\..", "dir"},
    {"dir\\.\\subdir\\.\\subsubdir\\.\\subsubsubdir\\..\\..\\..", "dir"},
    {"dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..", "dir"},
    {"\\dir\\.\\subdir\\..\\subsubdir\\..\\subsubsubdir\\..\\", "/dir/"},
    {"\\.\\..\\.\\..\\..\\..\\..\\..\\..\\..\\\\u0000\\..\\etc\\.\\passwd", "/etc/passwd"},
  };
  // clang-format on

  for (size_t i = 0; i < test_cases.size(); ++i) {
    auto& test_case = test_cases[i];
    std::string result;
    bool ret = normalize_path_win.evaluate(test_case.first, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, test_case.second);
    if (!ret || result != test_case.second) {
      std::cout << "Test case " << i << " failed: " << test_case.first << " -> " << result
                << std::endl;
    }
  }
}

TEST_F(TransformationTest, normalizePath) {
  const NormalizePath normalize_path;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = normalize_path.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(/path/to/file)";
  ret = normalize_path.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  // clang-format off
  std::vector<std::pair<std::string,std::string>> test_cases = {
    {".",""},
    {"./",""},
    {"./..", ".."},
    {"./../", "../"},
    {"..", ".."},
    {"../", "../"},
    {"../.", ".."},
    {".././", "../"},
    {"../..", "../.."},
    {"../../", "../../"},
    {"/dir/foo//bar", "/dir/foo/bar"},
    {"dir/foo//bar/", "dir/foo/bar/"},
    {"dir/../foo", "foo"},
    {"dir/../../foo", "../foo"},
    {"dir/./.././../../foo/bar", "../../foo/bar"},
    {"dir/./.././../../foo/bar/.", "../../foo/bar"},
    {"dir/./.././../../foo/bar/./", "../../foo/bar/"},
    {"dir/./.././../../foo/bar/..", "../../foo"},
    {"dir/./.././../../foo/bar/../", "../../foo/"},
    {"dir/./.././../../foo/bar/", "../../foo/bar/"},
    {"dir//.//..//.//..//..//foo//bar", "../../foo/bar"},
    {"dir//.//..//.//..//..//foo//bar//", "../../foo/bar/"},
    {"dir/subdir/subsubdir/subsubsubdir/../../..", "dir"},
    {"dir/./subdir/./subsubdir/./subsubsubdir/../../..", "dir"},
    {"dir/./subdir/../subsubdir/../subsubsubdir/..", "dir"},
    {"/dir/./subdir/../subsubdir/../subsubsubdir/../", "/dir/"},
    {"/./.././../../../../../../../\\u0000/../etc/./passwd", "/etc/passwd"},
  };
  // clang-format on

  for (size_t i = 0; i < test_cases.size(); ++i) {
    auto& test_case = test_cases[i];
    std::string result;
    bool ret = normalize_path.evaluate(test_case.first, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, test_case.second);
    if (!ret || result != test_case.second) {
      std::cout << "Test case " << i << " failed: " << test_case.first << " -> " << result
                << std::endl;
    }
  }
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
  const RemoveCommentsChar remove_comments_char;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = remove_comments_char.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This is /* comment */ a test)";
  ret = remove_comments_char.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is  comment  a test");

  data = R"(This is # comment a test)";
  ret = remove_comments_char.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is  comment a test");

  data = R"(This is -- comment a test)";
  ret = remove_comments_char.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is  comment a test");

  data = R"(This is <!-- comment a test)";
  ret = remove_comments_char.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is  comment a test");
}

TEST_F(TransformationTest, removeCommentChar) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, removeNulls) {
  const RemoveNulls remove_nulls;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = remove_nulls.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  {
    char data[] = "This is a test \0";
    ret = remove_nulls.evaluate({data, sizeof(data) - 1}, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "This is a test ");
  }

  {
    char data[] = "\0\0\0\0This is a test \0";
    ret = remove_nulls.evaluate({data, sizeof(data) - 1}, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "This is a test ");
  }

  {
    char data[] = "\0\0\0\0This\0\0\0\0 is\0\0\0\0 a\0\0\0\0 test \0";
    ret = remove_nulls.evaluate({data, sizeof(data) - 1}, result);
    EXPECT_TRUE(ret);
    EXPECT_EQ(result, "This is a test ");
  }
}

TEST_F(TransformationTest, removeWhitespace) {
  const RemoveWhitespace remove_whitespace;

  std::string data = R"(Thisisatest)";
  std::string result;
  bool ret = remove_whitespace.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This is a test)";
  ret = remove_whitespace.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "Thisisatest");

  data = "This \t\r\n\f\vis \t\r\n\f\va\xa0 test";
  ret = remove_whitespace.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "Thisisatest");
}

TEST_F(TransformationTest, replaceComments) {
  const ReplaceComments replace_comments;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = replace_comments.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This is a test /* comment */)";
  ret = replace_comments.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test  ");

  data = R"(This is /* comment */ a test)";
  ret = replace_comments.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is   a test");

  data = R"(This is /* comment a test)";
  ret = replace_comments.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is  ");

  data = R"(This is */ comment a test)";
  ret = replace_comments.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());
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
  const UrlDecodeUni url_decode_uni;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = url_decode_uni.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This%20is%20a%20test)";
  ret = url_decode_uni.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(This+is+a+test)";
  ret = url_decode_uni.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(%54%68is%20is%20a%20%74es%74)";
  ret = url_decode_uni.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(%u4E2D%u6587)";
  ret = url_decode_uni.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "\u4E2D\u6587");

  data = R"(%u4E2D+%u6587%20%u4E2D+%u6587%20)";
  ret = url_decode_uni.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "\u4E2D \u6587 \u4E2D \u6587 ");
}

TEST_F(TransformationTest, urlDecode) {
  const UrlDecode url_decode;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = url_decode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = R"(This%20is%20a%20test)";
  ret = url_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(This+is+a+test)";
  ret = url_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");

  data = R"(%54%68is%20is%20a%20%74es%74)";
  ret = url_decode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is a test");
}

TEST_F(TransformationTest, urlEncode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, utf8ToUnicode) {
  const Utf8ToUnicode utf8_to_unicode;

  std::string data = R"(This is a test)";
  std::string result;
  bool ret = utf8_to_unicode.evaluate(data, result);
  EXPECT_FALSE(ret);
  EXPECT_TRUE(result.empty());

  data = "\u4E2D\u6587";
  ret = utf8_to_unicode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "%u4e2d%u6587");

  data = "This is \u4E2D\u6587 ";
  ret = utf8_to_unicode.evaluate(data, result);
  EXPECT_TRUE(ret);
  EXPECT_EQ(result, "This is %u4e2d%u6587 ");
}
} // namespace Transformation
} // namespace SrSecurity