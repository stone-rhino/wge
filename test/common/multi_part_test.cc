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

#include "common/ragel/multi_part.h"

TEST(Common, multiPart) {
  SrSecurity::Common::Ragel::MultiPart multi_part;
  multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo1\r\n"
                  "header1: value1\r\n"
                  "\r\n"
                  "bar1\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo2\r\n"
                  "header2: value2\r\n"
                  "\r\n"
                  "bar2\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo3\r\n"
                  "header2: value3\r\n"
                  "\r\n"
                  "bar3\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=file1; filename=hello1\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=file2; filename=hello2\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=file3; filename=hello3\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld--",
                  3);
  auto error = multi_part.getError();
  EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
  auto& name_value_map = multi_part.getNameValue();
  auto& name_value_linked = multi_part.getNameValueLinked();
  EXPECT_EQ(name_value_map.size(), 3);
  EXPECT_EQ(name_value_linked.size(), 3);
  EXPECT_EQ(name_value_map.find("foo1")->second, "bar1\r\n");
  EXPECT_EQ(name_value_map.find("foo2")->second, "bar2\r\n");
  EXPECT_EQ(name_value_map.find("foo3")->second, "bar3\r\n");
  EXPECT_EQ(name_value_linked[0]->first, "foo1");
  EXPECT_EQ(name_value_linked[1]->first, "foo2");
  EXPECT_EQ(name_value_linked[2]->first, "foo3");
  EXPECT_EQ(name_value_linked[0]->second, "bar1\r\n");
  EXPECT_EQ(name_value_linked[1]->second, "bar2\r\n");
  EXPECT_EQ(name_value_linked[2]->second, "bar3\r\n");

  auto& name_filename_map = multi_part.getNameFileName();
  auto& name_filename_linked = multi_part.getNameFileNameLinked();
  EXPECT_EQ(name_filename_map.size(), 3);
  EXPECT_EQ(name_filename_linked.size(), 3);
  EXPECT_EQ(name_filename_map.find("file1")->second, "hello1");
  EXPECT_EQ(name_filename_map.find("file2")->second, "hello2");
  EXPECT_EQ(name_filename_map.find("file3")->second, "hello3");
  EXPECT_EQ(name_filename_linked[0]->first, "file1");
  EXPECT_EQ(name_filename_linked[1]->first, "file2");
  EXPECT_EQ(name_filename_linked[2]->first, "file3");
  EXPECT_EQ(name_filename_linked[0]->second, "hello1");
  EXPECT_EQ(name_filename_linked[1]->second, "hello2");
  EXPECT_EQ(name_filename_linked[2]->second, "hello3");

  auto& headers_map = multi_part.getHeaders();
  auto& headers_linked = multi_part.getHeadersLinked();
  EXPECT_EQ(headers_map.size(), 3);
  EXPECT_EQ(headers_linked.size(), 3);
  EXPECT_EQ(headers_map.find("header1")->second, "header1: value1");
  auto iter_range = headers_map.equal_range("header2");
  int count = 0;
  for(auto iter = iter_range.first; iter != iter_range.second; ++iter) {
    EXPECT_TRUE(iter->second == "header2: value2" || iter->second == "header2: value3");
    ++count;
  }
  EXPECT_EQ(count, 2);
  EXPECT_EQ(headers_linked[0]->first, "header1");
  EXPECT_EQ(headers_linked[1]->first, "header2");
  EXPECT_EQ(headers_linked[2]->first, "header2");
  EXPECT_EQ(headers_linked[0]->second, "header1: value1");
  EXPECT_EQ(headers_linked[1]->second, "header2: value2");
  EXPECT_EQ(headers_linked[2]->second, "header2: value3");
}

TEST(Common, multiPartError) {
  // ErrorType::BoundaryQuoted
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary="--helloworld"\r\n)", "");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::BoundaryWhitespace
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--hello world)", "");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::DataBefore
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)", "aa\r\n--helloworld\r\n");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::DataAfter
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=\"hello\"\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--aa");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::HeaderFolding
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=\"hello\";\r\n"
                    " filename=\"hello\"\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::LfLine
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\n"
                    "content-disposition: form-data; name=\"hello\"\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::MissingSemicolon
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data name=\"hello\"\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidQuoting
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=\"hello\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidQuoting
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=hello\"\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidQuoting
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=hel\"lo\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidPart
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=hello\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidPart
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=hello\r\n"
                    "\r\n"
                    "world\r\n");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::InvalidHeaderFolding
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data;name=hello\r\n"
                    "filename=hello\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::FileLimitExceeded
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=file1; filename=hello1\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=file2; filename=hello2\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld\r\n"
                    "content-disposition: form-data; name=file2; filename=hello3\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld--",
                    2);
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }

  // ErrorType::UnmatchedBoundary
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld2\r\n"
                    "content-disposition: form-data; name=hello\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld2--");
    auto error = multi_part.getError();
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::ReqbodyProcessorError));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryQuoted));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::BoundaryWhitespace));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataBefore));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::DataAfter));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::HeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::LfLine));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MissingSemicolon));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidQuoting));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidPart));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::InvalidHeaderFolding));
    EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::FileLimitExceeded));
    EXPECT_TRUE(error.get(SrSecurity::MultipartStrictError::ErrorType::UnmatchedBoundary));
  }
}