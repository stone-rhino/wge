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
#include <filesystem>

#include <gtest/gtest.h>

#include "transformation/transform_include.h"

#include "../../src/common/duration.h"

#define NORM(x) std::filesystem::path(x).lexically_normal().string()

#define LINUX2WIN(x)                                                                               \
  ([]() {                                                                                          \
    std::string s = x;                                                                             \
    std::replace(s.begin(), s.end(), '/', '\\');                                                   \
    return s;                                                                                      \
  }())

#define CASE_LINUX(ret, input)                                                                     \
  { ret, input, NORM(input) }

#define CASE_WIN(ret, input)                                                                       \
  { ret, LINUX2WIN(input), NORM(input) }

namespace Wge {
namespace Test {
namespace Transformation {
class TransformationTest : public ::testing::Test {};

struct TestCase {
  bool result;
  std::string input_;
  std::string output_;
};

template <class T> void evaluate(const std::vector<TestCase>& test_cases) {
  const T transform;
  for (size_t i = 0; i < test_cases.size(); ++i) {
    const auto& test_case = test_cases[i];
    std::string result;
    bool ret = transform.evaluate(test_case.input_, result);
    if (ret != test_case.result) {
      std::cout << "Test case index: " << i << std::endl;
    }
    EXPECT_EQ(ret, test_case.result);
    if (ret) {
      if (result != test_case.output_) {
        std::cout << "Test case index: " << i << std::endl;
      }
      EXPECT_EQ(result, test_case.output_);
    } else {
      if (!result.empty()) {
        std::cout << "Test case index: " << i << std::endl;
      }
      EXPECT_TRUE(result.empty());
    }
  }
}

template <class T> void evaluateStream(const std::vector<TestCase>& test_cases, size_t step) {
  const T transform;
  for (size_t i = 0; i < test_cases.size(); ++i) {
    const auto& test_case = test_cases[i];
    auto state = transform.newStream();
    std::string output;
    for (size_t j = 0; j < test_case.input_.size();) {
      size_t input_step = std::min(step, test_case.input_.size() - j);
      std::string_view input(&test_case.input_[j], input_step);
      auto stream_result = transform.evaluateStream(input, output, *state,
                                                    j + input_step >= test_case.input_.size());
      EXPECT_NE(stream_result, Wge::Transformation::StreamResult::INVALID_INPUT);
      j += input_step;
    }
    if (output != test_case.output_) {
      std::cout << "Test case index: " << i << std::endl;
    }
    EXPECT_EQ(output, test_case.output_);
  }
}

template <class T> void evaluateStream(const std::vector<TestCase>& test_cases) {
  static constexpr size_t max_step = 10;
  for (size_t step = 1; step <= max_step; ++step) {
    evaluateStream<T>(test_cases, step);
  }
}

TEST_F(TransformationTest, base64DecodeExt) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, base64Decode) {
  const std::vector<TestCase> test_cases = {
      {true, "VGhpcyBpcyBhIHRlc3Q=", "This is a test"},
      {true, "VGhpcyBpcyBhIHRlc3Q", "This is a test"},
      {true, R"(VGhpcy(Bp)cyB#hIH@Rl!c3Q=)", "This is a test"},
      {true, "VGhpcyBpcyBhIHRlc3=VGhpcyBpcyBhIHRlc3", "This is a tes"},
  };

  evaluate<Wge::Transformation::Base64Decode>(test_cases);
  evaluateStream<Wge::Transformation::Base64Decode>(test_cases);
}

TEST_F(TransformationTest, base64Encode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, cmdLine) {
  const std::vector<TestCase> test_cases = {
      // Test that prescan is working, and that will not copy if there is no transformation
      {false, "this is a test", "this is a test"},
      // Test that prescan is working, and that will hold the token if there is a transformation
      {true, "this        is a ;;;;;;;;;test data", "this is a test data"},
      // Deleting all backslashes [\]
      {true, R"(This is a \test\ \data\)", "this is a test data"},
      // Deleting all double quotes ["]
      {true, R"(this is a \"test\ \"data\)", "this is a test data"},
      // Deleting all single quotes [']
      {true, R"(this is a \"test'\ \"data'\)", "this is a test data"},
      // Deleting all carets [^]
      {true, R"(this is a \"te^st'\ \"da^ta'\)", "this is a test data"},
      // Deleting spaces before a slash /
      {true, R"(this is a \"te^st'\           /\"da^ta'\)", "this is a test/data"},
      // Deleting spaces before an open parentesis [(]
      {true, R"(this is a \"te^st'\           /          (\"da^ta'\)", "this is a test/(data"},
      // Replacing all commas [,] and semicolon [;] into a space
      {true, R"(this is a \"te^st'\           /          (,\"da^t;a'\)", "this is a test/( dat a"},
      // Replacing all multiple spaces (including tab, newline, etc.) into one space
      {true, "this is a \\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a'\\",
       "this is a test/( dat a"},
      // Transform all characters to lowercase
      {true, "this is a \\\"te^st'\\           /          (,\\\"da^t;\t\r\n  a_HELLO'\\",
       "this is a test/( dat a_hello"},
      // Deleting all double quotes ["] and deleting spaces before a slash /
      {true, R"(BX4;HyzokkcX "/fQq;AY      V b)", "bx4 hyzokkcx/fqq ay v b"},
  };

  evaluate<Wge::Transformation::CmdLine>(test_cases);
  evaluateStream<Wge::Transformation::CmdLine>(test_cases);
}

TEST_F(TransformationTest, compressWhiteSpace) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "This   is   a   test", "This is a test"},
      {true, "This \f\t\n\r\v\xa0 is \f\t\n\r\v\xa0 a \f\t\n\r\v\xa0 test", "This is a test"}};

  evaluate<Wge::Transformation::CompressWhiteSpace>(test_cases);
  evaluateStream<Wge::Transformation::CompressWhiteSpace>(test_cases);
}

TEST_F(TransformationTest, cssDecode) {
  // clang-format off
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, R"(This\ is\ a\ test)", "This is a test"},
      {true, R"(T\hi\s is a test)", "This is a test"},
      {true, R"(This\ is\ a\ test\)", "This is a test"},
      {true, R"(This\ is\ a\ test\ \)", "This is a test "},
      {true, R"(\1254\3468 is\ is\ a\ test\ \ \)", "This is a test  "},
      {true, R"(\12354\123468is\ is\ a\ test\ \ \)", "This is a test  "},
      {true, R"(\12354\123468\6is\ is\ a\ test\ \ \)", "Th\u0006is is a test  "},
      {false, std::string("Test\u0000Case", 9), std::string("Test\u0000Case", 9)},
      {true, std::string("test\\a\\b\\f\\n\\r\\t\\v\\?\\'\\\"\\\u0000\\12\\123\\1234\\12345\\123456\\ff01\\ff5e\\\n\\\u0000  string", 73),std::string("test\n\u000b\u000fnrtv?'\"\u0000\u0012#4EV!~\u0000  string", 31)},
      {true, std::string("\\1A\\1 A\\1234567\\123456 7\\1x\\1 x", 31), std::string("\u001a\u0001AV7V7\u0001x\u0001x", 11)}
  };
  // clang-format on

  evaluate<Wge::Transformation::CssDecode>(test_cases);
  evaluateStream<Wge::Transformation::CssDecode>(test_cases);
}

TEST_F(TransformationTest, escapeSeqDecode) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, R"(This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab \101 \01 \1)",
       "This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab A \1 \1"},
  };

  evaluate<Wge::Transformation::EscapeSeqDecode>(test_cases);
  evaluateStream<Wge::Transformation::EscapeSeqDecode>(test_cases);
}

TEST_F(TransformationTest, hexDecode) {
  const std::vector<TestCase> test_cases = {
      {false, "G5468697320697320612074657374", ""},
      {true, "a", "\n"},
      {true, "5468G697320697320612074657374", "Th"},
      {true, "5468697320697320612074657374", "This is a test"}};

  evaluate<Wge::Transformation::HexDecode>(test_cases);
  evaluateStream<Wge::Transformation::HexDecode>(test_cases);
}

TEST_F(TransformationTest, hexEncode) {
  const std::vector<TestCase> test_cases = {
      {true, "This is a test", "5468697320697320612074657374"}};

  evaluate<Wge::Transformation::HexEncode>(test_cases);
}

TEST_F(TransformationTest, htmlEntityDecode) {
  // clang-format off
  const std::vector<TestCase> test_cases = {
    {false, "This is a test data", "This is a test data"},
    {true,"&#x54;&#x68;&#x69;&#x73;&#x20;&#x69;&#x73;&#x20;&#x61;&#x20;&#x74;&#x65;&#x73;&#x74;", "This is a test"},
    {true,"&#x000000000054;&#x000068;&#x69;&#x73;&#x20;&#x69;&#x73;&#x20;&#x61;&#x20;&#x74;&#x65;&#x73;&#x74;", "This is a test"},
    {true,"&#84;&#104;&#105;&#115;&#32;&#105;&#115;&#32;&#97;&#32;&#116;&#101;&#115;&#116;", "This is a test"},
    {true,"&#000000000084;&#0000104;&#105;&#115;&#32;&#105;&#115;&#32;&#97;&#32;&#116;&#101;&#115;&#116;", "This is a test"},
    {true,"&#x54;his is a test", "This is a test"},
    {true,"&#84;his is a test", "This is a test"},
    {true,"&#x54;his is a test", "This is a test"},
    {true,"&#84;his is a test", "This is a test"},
    {true,"&amp; &lt; &gt; &quot; &apos; &nbsp;", "& < > \" '  "},
    {true,"&amp;&apos;this&apos;&nbsp;&quot;is&quot;&nbsp;a&nbsp;&lt;te&#115;&#116;&gt;", "&'this' \"is\" a <test>"},
    {true,"xss src=&#x6a&#x61&#x76&#x61&#x73&#x63&#x72&#x69&#x70&#x74&#x3a&#x61&#x6c&#x65&#x72&#x74&#x28&#x27&#x58&#x53&#x53&#x27&#x29>","xss src=javascript:alert('XSS')>"},
    // Test for not valid html entity
    {true,"&amp; &lt; &gt; &quot; &apos; &nbsp; &notValid;","& < > \" '   &notValid;"},
    // Test for not valid html entity with invalid number
    {true,"&#23234234234234;&#x6a","&#23234234234234;j"},
    {true,"&#000000000023234234234234;&#x6a","&#000000000023234234234234;j"}
  };
  // clang-format on

  evaluate<Wge::Transformation::HtmlEntityDecode>(test_cases);
  evaluateStream<Wge::Transformation::HtmlEntityDecode>(test_cases);
}

TEST_F(TransformationTest, jsDecode) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test data", "This is a test data"},
      {true, R"(This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab \101 \01 \1)",
       "This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab A \1 \1"},
      {true,
       R"(\u0054\u0068\u0069\u0073\u0020\u0069\u0073\u0020\u0061\u0020\u0074\u0065\u0073\u0074)",
       "\u0054\u0068\u0069\u0073 \u0069\u0073 \u0061 \u0074\u0065\u0073\u0074"}};

  evaluate<Wge::Transformation::JsDecode>(test_cases);
  evaluateStream<Wge::Transformation::JsDecode>(test_cases);
}

TEST_F(TransformationTest, length) {
  const std::vector<TestCase> test_cases = {
      {true, "This is a test", "14"},
      {true, "This is a test data", "19"},
      {true, R"(This is a test data. \a \b \f \n \r \t \v \\ \? \' \" \xab \101 \01 \1)", "70"}};

  evaluate<Wge::Transformation::Length>(test_cases);
  evaluateStream<Wge::Transformation::Length>(test_cases);
}

TEST_F(TransformationTest, lowercase) {
  const std::vector<TestCase> test_cases = {
      {false, "this is a test", "this is a test"},
      {true, "THIS IS A TEST", "this is a test"},
      {true, R"(ThiS iS A TeSt~!@#$%^&*()_+)", "this is a test~!@#$%^&*()_+"}};

  evaluate<Wge::Transformation::LowerCase>(test_cases);
  evaluateStream<Wge::Transformation::LowerCase>(test_cases);
}

TEST_F(TransformationTest, md5) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, normalisePathWin) {
#define CASE CASE_WIN
  const std::vector<TestCase> test_cases = {
      CASE(false, "This is a test"),                                       // 0
      CASE(true, "/path/to/file"),                                         // 1
      CASE(false, "."),                                                    // 2
      CASE(true, "./"),                                                    // 3
      CASE(true, "./.."),                                                  // 4
      CASE(true, "./../"),                                                 // 5
      CASE(false, ".."),                                                   // 6
      CASE(true, "../"),                                                   // 7
      CASE(true, "../."),                                                  // 8
      CASE(true, ".././"),                                                 // 9
      CASE(true, "../.."),                                                 // 10
      CASE(true, "../../"),                                                // 11
      CASE(true, ".atom/"),                                                // 12
      CASE(true, "./dir/.atom."),                                          // 13
      CASE(true, "./dir/.atom./"),                                         // 14
      CASE(true, "../dir/.atom."),                                         // 15
      CASE(true, "./dir/.atom../"),                                        // 16
      CASE(true, "../dir/.atom.."),                                        // 17
      CASE(true, "../dir/.atom./"),                                        // 18
      CASE(true, ".../dir/.atom."),                                        // 19
      CASE(true, ".../dir/.atom./"),                                       // 20
      CASE(true, "..../dir/.atom."),                                       // 21
      CASE(true, "dir/.atom./"),                                           // 22
      CASE(true, "dir.atom/"),                                             // 23
      CASE(true, "/dir/foo//bar"),                                         // 24
      CASE(true, "dir/foo//bar/"),                                         // 25
      CASE(true, "dir/../"),                                               // 26
      CASE(true, "dir/.."),                                                // 27
      CASE(true, "dir/../foo"),                                            // 28
      CASE(true, "dir/../../foo"),                                         // 29
      CASE(true, "dir/./.././../../foo/bar"),                              // 30
      CASE(true, "dir/./.././../../foo/bar/."),                            // 31
      CASE(true, "dir/./.././../../foo/bar/./"),                           // 32
      CASE(true, "dir/./.././../../foo/bar/.."),                           // 33
      CASE(true, "dir/./.././../../foo/bar/../"),                          // 34
      CASE(true, "dir/./.././../../foo/bar/"),                             // 35
      CASE(true, "dir//.//..//.//..//..//foo//bar"),                       // 36
      CASE(true, "dir//.//..//.//..//..//foo//bar//"),                     // 37
      CASE(true, "dir/subdir/subsubdir/subsubsubdir/../../.."),            // 38
      CASE(true, "dir/./subdir/./subsubdir/./subsubsubdir/../../.."),      // 39
      CASE(true, "dir/./subdir/../subsubdir/../subsubsubdir/.."),          // 40
      CASE(true, "/dir/./subdir/../subsubdir/../subsubsubdir/../"),        // 41
      CASE(true, "/./.././../../../../../../../\\u0000/../etc/./passwd"),  // 42
      CASE(true, "/../.././../../../../../../../\\u0000/../etc/./passwd"), // 43
      CASE(true, "/etc/.."),                                               // 44
      CASE(true, "/.."),                                                   // 45
      CASE(true, "////////etc/////"),                                      // 46
      CASE(true, "////....test"),                                          // 47
  };
#undef CASE

  evaluate<Wge::Transformation::NormalisePathWin>(test_cases);
  evaluateStream<Wge::Transformation::NormalisePathWin>(test_cases);

  // Test NormalizePathWin
  evaluate<Wge::Transformation::NormalizePathWin>(test_cases);
  evaluateStream<Wge::Transformation::NormalizePathWin>(test_cases);
}

TEST_F(TransformationTest, normalisePath) {
#define CASE CASE_LINUX
  const std::vector<TestCase> test_cases = {
      CASE(false, "This is a test"),                                       // 0
      CASE(false, "/path/to/file"),                                        // 1
      CASE(false, "."),                                                    // 2
      CASE(true, "./"),                                                    // 3
      CASE(true, "./.."),                                                  // 4
      CASE(true, "./../"),                                                 // 5
      CASE(false, ".."),                                                   // 6
      CASE(true, "../"),                                                   // 7
      CASE(true, "../."),                                                  // 8
      CASE(true, ".././"),                                                 // 9
      CASE(true, "../.."),                                                 // 10
      CASE(true, "../../"),                                                // 11
      CASE(false, ".atom/"),                                               // 12
      CASE(true, "./dir/.atom."),                                          // 13
      CASE(true, "./dir/.atom./"),                                         // 14
      CASE(false, "../dir/.atom."),                                        // 15
      CASE(true, "./dir/.atom../"),                                        // 16
      CASE(false, "../dir/.atom.."),                                       // 17
      CASE(false, "../dir/.atom./"),                                       // 18
      CASE(false, ".../dir/.atom."),                                       // 19
      CASE(false, ".../dir/.atom./"),                                      // 20
      CASE(false, "..../dir/.atom."),                                      // 21
      CASE(false, "dir/.atom./"),                                          // 22
      CASE(false, "dir.atom/"),                                            // 23
      CASE(true, "/dir/foo//bar"),                                         // 24
      CASE(true, "dir/foo//bar/"),                                         // 25
      CASE(true, "dir/../"),                                               // 26
      CASE(true, "dir/.."),                                                // 27
      CASE(true, "dir/../foo"),                                            // 28
      CASE(true, "dir/../../foo"),                                         // 29
      CASE(true, "dir/./.././../../foo/bar"),                              // 30
      CASE(true, "dir/./.././../../foo/bar/."),                            // 31
      CASE(true, "dir/./.././../../foo/bar/./"),                           // 32
      CASE(true, "dir/./.././../../foo/bar/.."),                           // 33
      CASE(true, "dir/./.././../../foo/bar/../"),                          // 34
      CASE(true, "dir/./.././../../foo/bar/"),                             // 35
      CASE(true, "dir//.//..//.//..//..//foo//bar"),                       // 36
      CASE(true, "dir//.//..//.//..//..//foo//bar//"),                     // 37
      CASE(true, "dir/subdir/subsubdir/subsubsubdir/../../.."),            // 38
      CASE(true, "dir/./subdir/./subsubdir/./subsubsubdir/../../.."),      // 39
      CASE(true, "dir/./subdir/../subsubdir/../subsubsubdir/.."),          // 40
      CASE(true, "/dir/./subdir/../subsubdir/../subsubsubdir/../"),        // 41
      CASE(true, "/./.././../../../../../../../\\u0000/../etc/./passwd"),  // 42
      CASE(true, "/../.././../../../../../../../\\u0000/../etc/./passwd"), // 43
      CASE(true, "/etc/.."),                                               // 44
      CASE(true, "/.."),                                                   // 45
      CASE(true, "////////etc/////"),                                      // 46
      CASE(true, "////....test"),                                          // 47
  };
#undef CASE

  // for(const auto& test_case : test_cases) {
  //   std::cout << test_case.input_ << std::endl;
  //   std::cout << test_case.output_ << std::endl << std::endl;
  // }

  evaluate<Wge::Transformation::NormalisePath>(test_cases);
  evaluateStream<Wge::Transformation::NormalisePath>(test_cases);

  // Test NormalizePath
  evaluate<Wge::Transformation::NormalizePath>(test_cases);
  evaluateStream<Wge::Transformation::NormalizePath>(test_cases);
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
  std::vector<TestCase> test_cases = {{false, "This is a test", "This is a test"},
                                      {true, "#This is a test", ""},
                                      {true, "--This is a test", ""},
                                      {true, "This is /* comment */ a test", "This is  a test"},
                                      {true, "# comment\nThis is # comment a test", "This is "},
                                      {true, "This is -- comment a test", "This is "},
                                      {true, "This is <!-- comment a test", "This is "}};

  evaluate<Wge::Transformation::RemoveComments>(test_cases);
  evaluateStream<Wge::Transformation::RemoveComments>(test_cases);
}

TEST_F(TransformationTest, removeCommentChar) {
  std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "This is /* comment */ a test", "This is  comment  a test"},
      {true, "This is # comment a test", "This is  comment a test"},
      {true, "This is -- comment a test", "This is  comment a test"},
      {true, "This is <!-- comment a test", "This is  comment a test"}};

  evaluate<Wge::Transformation::RemoveCommentsChar>(test_cases);
  evaluateStream<Wge::Transformation::RemoveCommentsChar>(test_cases);
}

TEST_F(TransformationTest, removeNulls) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, {"This is a test \0", 16}, "This is a test "},
      {true, {"\0\0\0\0This is a test \0", 20}, "This is a test "},
      {true, {"\0\0\0\0This\0\0\0\0 is\0\0\0\0 a\0\0\0\0 test \0", 32}, "This is a test "},
  };

  evaluate<Wge::Transformation::RemoveNulls>(test_cases);
  evaluateStream<Wge::Transformation::RemoveNulls>(test_cases);
}

TEST_F(TransformationTest, removeWhitespace) {
  const std::vector<TestCase> test_cases = {
      {false, "Thisisatest", "Thisisatest"},
      {true, "This is a test", "Thisisatest"},
      {true, "This \t\r\n\f\vis \t\r\n\f\va\xa0 test", "Thisisatest"}};

  evaluate<Wge::Transformation::RemoveWhitespace>(test_cases);
  evaluateStream<Wge::Transformation::RemoveWhitespace>(test_cases);
}

TEST_F(TransformationTest, replaceComments) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "This is a test /* comment */", "This is a test  "},
      {true, "This is /* comment */ a test", "This is   a test"},
      {true, "This is /* comment a test", "This is  "},
      {false, "This is */ comment a test", "This is */ comment a test"}};

  evaluate<Wge::Transformation::ReplaceComments>(test_cases);
  evaluateStream<Wge::Transformation::ReplaceComments>(test_cases);
}

TEST_F(TransformationTest, replaceNulls) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, {"This is a test \0", 16}, "This is a test  "},
      {true, {"\0\0\0\0This is a test \0", 20}, " This is a test  "},
      {true, {"\0\0\0\0This\0\0\0\0 is\0\0\0\0 a\0\0\0\0 test \0", 32}, " This  is  a  test  "}};

  evaluate<Wge::Transformation::ReplaceNulls>(test_cases);
  evaluateStream<Wge::Transformation::ReplaceNulls>(test_cases);
}

TEST_F(TransformationTest, sha1) {
  const std::vector<TestCase> test_cases = {
      {true, "This is a test", "a54d88e06612d820bc3be72877c74f257b561b19"},
      {true, "This is a test data", "80ce9fbcd1461f1b81357159f1f261eddd0681dd"}};

  evaluate<Wge::Transformation::Sha1>(test_cases);
  evaluateStream<Wge::Transformation::Sha1>(test_cases);
}

TEST_F(TransformationTest, sqlHexDecode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, trimLeft) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "\t\n\r\f\v\x20 This is a test \t\n\r\f\v\x20", "This is a test \t\n\r\f\v\x20"},
      {true, "\t\n\r\f\v\x20 ", ""}};

  evaluate<Wge::Transformation::TrimLeft>(test_cases);
  evaluateStream<Wge::Transformation::TrimLeft>(test_cases);
}

TEST_F(TransformationTest, trimRight) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "\t\n\r\f\v\x20 This is a test \t\n\r\f\v\x20", "\t\n\r\f\v\x20 This is a test"},
      {true, "\t\n\r\f\v\x20 ", ""}};

  evaluate<Wge::Transformation::TrimRight>(test_cases);

  const std::vector<TestCase> test2_cases = {
      {false, "This is a test", "This is a test"},
      {true, "\t\n\r\f\v\x20 This is a test \t\n\r\f\v\x20", "       This is a test"},
      {true, "\t\n\r\f\v\x20 ", ""}};
  evaluateStream<Wge::Transformation::TrimRight>(test2_cases);
}

TEST_F(TransformationTest, trim) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "\t\n\r\f\v\x20 This is a test \t\n\r\f\v\x20", "This is a test"},
      {true, "\t\n\r\f\v\x20 ", ""}};

  evaluate<Wge::Transformation::Trim>(test_cases);
  evaluateStream<Wge::Transformation::Trim>(test_cases);
}

TEST_F(TransformationTest, upperCase) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, urlDecodeUni) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "This%20is%20a%20test", "This is a test"},
      {true, "This+is+a+test", "This is a test"},
      {true, "%54%68is%20is%20a%20%74es%74", "This is a test"},
      {true, "%u4E2D%u6587", "\x2D\x87"},
      {true, "%u4E2D+%u6587%20%u4E2D+%u6587%20", "\x2D \x87 \x2D \x87 "},
      {true, "%uff1cscript%uff1ealert(%uff07XSS%uff07);%uff1c/script%uff1e",
       "<script>alert('XSS');</script>"}};

  evaluate<Wge::Transformation::UrlDecodeUni>(test_cases);
  evaluateStream<Wge::Transformation::UrlDecodeUni>(test_cases);
}

TEST_F(TransformationTest, urlDecode) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "This%20is%20a%20test", "This is a test"},
      {true, "This+is+a+test", "This is a test"},
      {true, "%54%68is%20is%20a%20%74es%74", "This is a test"}};

  evaluate<Wge::Transformation::UrlDecode>(test_cases);
  evaluateStream<Wge::Transformation::UrlDecode>(test_cases);
}

TEST_F(TransformationTest, urlEncode) {
  // TODO(zhouyu 2025-03-21): Implement this test
}

TEST_F(TransformationTest, utf8ToUnicode) {
  const std::vector<TestCase> test_cases = {
      {false, "This is a test", "This is a test"},
      {true, "\u4E2D\u6587", "%u4e2d%u6587"},
      {true, "This is \u4E2D\u6587 ", "This is %u4e2d%u6587 "}};

  evaluate<Wge::Transformation::Utf8ToUnicode>(test_cases);
  evaluateStream<Wge::Transformation::Utf8ToUnicode>(test_cases);
}
} // namespace Transformation
} // namespace Test
} // namespace Wge