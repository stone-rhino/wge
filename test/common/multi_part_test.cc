#include <gtest/gtest.h>

#include "common/ragel/multi_part.h"

TEST(Common, multiPart) {
  SrSecurity::Common::Ragel::MultiPart multi_part;
  multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo1\r\n"
                  "\r\n"
                  "bar1\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo2\r\n"
                  "\r\n"
                  "bar2\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; name=foo3\r\n"
                  "\r\n"
                  "bar3\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; filename=hello1\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; filename=hello2\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld\r\n"
                  "content-disposition: form-data; filename=hello3\r\n"
                  "\r\n"
                  "world\r\n"
                  "--helloworld--",
                  3);
  auto error = multi_part.getError();
  EXPECT_FALSE(error.get(SrSecurity::MultipartStrictError::ErrorType::MultipartStrictError));
  EXPECT_EQ(multi_part.get().size(), 3);
  EXPECT_EQ(multi_part.getLinked().size(), 3);
  EXPECT_EQ(multi_part.get().at("foo1"), "bar1\r\n");
  EXPECT_EQ(multi_part.get().at("foo2"), "bar2\r\n");
  EXPECT_EQ(multi_part.get().at("foo3"), "bar3\r\n");
  EXPECT_EQ(multi_part.getLinked()[0]->first, "foo1");
  EXPECT_EQ(multi_part.getLinked()[1]->first, "foo2");
  EXPECT_EQ(multi_part.getLinked()[2]->first, "foo3");
  EXPECT_EQ(multi_part.getLinked()[0]->second, "bar1\r\n");
  EXPECT_EQ(multi_part.getLinked()[1]->second, "bar2\r\n");
  EXPECT_EQ(multi_part.getLinked()[2]->second, "bar3\r\n");
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
  }

  // ErrorType::HeaderFolding
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data;\r\n"
                    " name=\"hello\"\r\n"
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
  }

  // ErrorType::InvalidHeaderFolding
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data;\r\n"
                    "name=hello\r\n"
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
  }

  // ErrorType::FileLimitExceeded
  {
    SrSecurity::Common::Ragel::MultiPart multi_part;
    multi_part.init(R"(multipart/form-data; boundary=--helloworld)",
                    "--helloworld\r\n"
                    "content-disposition: form-data; filename=hello1\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld\r\n"
                    "content-disposition: form-data; filename=hello2\r\n"
                    "\r\n"
                    "world\r\n"
                    "--helloworld\r\n"
                    "content-disposition: form-data; filename=hello3\r\n"
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
  }
}