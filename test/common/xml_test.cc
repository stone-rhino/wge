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
#include <string_view>

#include <gtest/gtest.h>
#include <libxml/xmlstring.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/xpath.h>

#include "common/duration.h"
#include "common/ragel/xml.h"

class XmlTest : public ::testing::Test {

protected:
  static constexpr std::string_view xml_ = R"(<bookstore>
  <book id="1" category="fiction">
    <title lang="en">XML Guide</title>
    <author>John Doe</author>
  </book>
  <book id="2" category="non-fiction">
    <title lang="fr">Advanced XML</title>
    <author>Jane Smith</author>
  </book>
</bookstore>)";

  // Test for the CDATA section in XML
  static constexpr std::string_view xml2_ = R"(<?xml version='1.0' encoding='iso-8859-1'?>
<methodCall>
    <methodName>pfsense.exec_php</methodName>
    <params>
        <param><![CDATA[<?php system('whoami'); ?>]]></param>
    </params>
</methodCall>)";

  // Test for multiple text nodes
  static constexpr std::string_view xml3_ = R"(<?xml version='1.0' encoding='iso-8859-1'?>
<methodCall>
    <methodName>pfsense.exec_php</methodName>
    <params>
        <param><value><string>csc3232r</string></value></param>
        <param><value><string>exec('echo \\'<pre> <?php $res = system($_GET["cmd"]); echo $res ?> </pre>\\' > /usr/local/www/[random_file].php');</string></value></param>
    </params>
</methodCall>)";

  std::forward_list<std::string> buffer_;
};

TEST_F(XmlTest, ragle) {
  Wge::Common::Ragel::Xml xml_parser;
  xml_parser.init(xml_, buffer_);
  auto& attributes = xml_parser.getAttributes();
  auto& tags = xml_parser.getTags();
  auto& tag_values_str = xml_parser.getTagValuesStr();
  EXPECT_EQ(attributes.size(), 6);
  EXPECT_EQ(tags.size(), 7);
  EXPECT_EQ(attributes[0].first, "id");
  EXPECT_EQ(attributes[0].second, "1");
  EXPECT_EQ(attributes[1].first, "category");
  EXPECT_EQ(attributes[1].second, "fiction");
  EXPECT_EQ(attributes[2].first, "lang");
  EXPECT_EQ(attributes[2].second, "en");
  EXPECT_EQ(attributes[3].first, "id");
  EXPECT_EQ(attributes[3].second, "2");
  EXPECT_EQ(attributes[4].first, "category");
  EXPECT_EQ(attributes[4].second, "non-fiction");
  EXPECT_EQ(attributes[5].first, "lang");
  EXPECT_EQ(attributes[5].second, "fr");
  EXPECT_EQ(tags[0].first, "bookstore");
  EXPECT_EQ(tags[0].second, "");
  EXPECT_EQ(tags[1].first, "book");
  EXPECT_EQ(tags[1].second, "");
  EXPECT_EQ(tags[2].first, "title");
  EXPECT_EQ(tags[2].second, "XML Guide");
  EXPECT_EQ(tags[3].first, "author");
  EXPECT_EQ(tags[3].second, "John Doe");
  EXPECT_EQ(tags[4].first, "book");
  EXPECT_EQ(tags[4].second, "");
  EXPECT_EQ(tags[5].first, "title");
  EXPECT_EQ(tags[5].second, "Advanced XML");
  EXPECT_EQ(tags[6].first, "author");
  EXPECT_EQ(tags[6].second, "Jane Smith");
  EXPECT_EQ(tag_values_str, "\n"
                            "  \n"
                            "    XML Guide\n"
                            "    John Doe\n"
                            "  \n"
                            "  \n"
                            "    Advanced XML\n"
                            "    Jane Smith\n"
                            "  \n");
}

TEST_F(XmlTest, ragle2) {
  Wge::Common::Ragel::Xml xml_parser;
  xml_parser.init(xml2_, buffer_);
  auto& attributes = xml_parser.getAttributes();
  auto& tags = xml_parser.getTags();
  auto& tag_values_str = xml_parser.getTagValuesStr();
  EXPECT_EQ(attributes.size(), 0);
  EXPECT_EQ(tags.size(), 4);
  EXPECT_EQ(tags[0].first, "methodCall");
  EXPECT_EQ(tags[0].second, "");
  EXPECT_EQ(tags[1].first, "methodName");
  EXPECT_EQ(tags[1].second, "pfsense.exec_php");
  EXPECT_EQ(tags[2].first, "params");
  EXPECT_EQ(tags[2].second, "");
  EXPECT_EQ(tags[3].first, "param");
  EXPECT_EQ(tags[3].second, "<?php system('whoami'); ?>");

  EXPECT_EQ(tag_values_str, "\n"
                            "    pfsense.exec_php\n"
                            "    \n"
                            "        <?php system('whoami'); ?>\n"
                            "    \n");
}

TEST_F(XmlTest, ragle3) {
  Wge::Common::Ragel::Xml xml_parser;
  xml_parser.init(xml3_, buffer_);
  auto& attributes = xml_parser.getAttributes();
  auto& tags = xml_parser.getTags();
  auto& tag_values_str = xml_parser.getTagValuesStr();
  EXPECT_EQ(attributes.size(), 0);
  EXPECT_EQ(tags.size(), 10);
  EXPECT_EQ(tags[0].first, "methodCall");
  EXPECT_EQ(tags[0].second, "");
  EXPECT_EQ(tags[1].first, "methodName");
  EXPECT_EQ(tags[1].second, "pfsense.exec_php");
  EXPECT_EQ(tags[2].first, "params");
  EXPECT_EQ(tags[2].second, "");
  EXPECT_EQ(tags[3].first, "param");
  EXPECT_EQ(tags[3].second, "");
  EXPECT_EQ(tags[4].first, "value");
  EXPECT_EQ(tags[4].second, "");
  EXPECT_EQ(tags[5].first, "string");
  EXPECT_EQ(tags[5].second, "csc3232r");
  EXPECT_EQ(tags[6].first, "param");
  EXPECT_EQ(tags[6].second, "");
  EXPECT_EQ(tags[7].first, "value");
  EXPECT_EQ(tags[7].second, "");
  EXPECT_EQ(tags[8].first, "string");
  EXPECT_EQ(tags[8].second, R"EOF(exec('echo \\'\\' > /usr/local/www/[random_file].php');)EOF");
  EXPECT_EQ(tags[9].first, "pre");
  EXPECT_EQ(tags[9].second, "");

  EXPECT_EQ(tag_values_str, "\n"
                            "    pfsense.exec_php\n"
                            "    \n"
                            "        csc3232r\n"
                            R"(        exec('echo \\'  \\' > /usr/local/www/[random_file].php');)"
                            "\n"
                            "    \n");
}

TEST_F(XmlTest, libxml) {
  std::string xpath1 = "/*";
  std::string xpath2 = "//@*";

  xmlDocPtr doc = xmlParseMemory(xml_.data(), xml_.size());
  ASSERT_NE(doc, nullptr);

  xmlXPathContextPtr xpath_ctx = xmlXPathNewContext(doc);
  ASSERT_NE(xpath_ctx, nullptr);

  xmlXPathObjectPtr xpath_obj1 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath1.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj1, nullptr);

  xmlXPathObjectPtr xpath_obj2 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath2.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj2, nullptr);

  std::vector<std::string> tag_values;
  for (int i = 0; i < xpath_obj1->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj1->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj1->nodesetval->nodeTab[i]));
    if (content != NULL) {
      tag_values.emplace_back(content);
      xmlFree(content);
    }
  }

  std::vector<std::string> attr_values;
  for (int i = 0; i < xpath_obj2->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj2->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj2->nodesetval->nodeTab[i]));
    if (content != NULL) {
      attr_values.emplace_back(content);
      xmlFree(content);
    }
  }

  EXPECT_EQ(attr_values.size(), 6);
  EXPECT_EQ(tag_values.size(), 1);
  EXPECT_EQ(attr_values[0], "1");
  EXPECT_EQ(attr_values[1], "fiction");
  EXPECT_EQ(attr_values[2], "en");
  EXPECT_EQ(attr_values[3], "2");
  EXPECT_EQ(attr_values[4], "non-fiction");
  EXPECT_EQ(attr_values[5], "fr");
  EXPECT_EQ(tag_values[0], "\n"
                           "  \n"
                           "    XML Guide\n"
                           "    John Doe\n"
                           "  \n"
                           "  \n"
                           "    Advanced XML\n"
                           "    Jane Smith\n"
                           "  \n");

  xmlXPathFreeObject(xpath_obj1);
  xmlXPathFreeObject(xpath_obj2);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
}

TEST_F(XmlTest, libxml2) {
  std::string xpath1 = "/*";
  std::string xpath2 = "//@*";

  xmlDocPtr doc = xmlParseMemory(xml2_.data(), xml2_.size());
  ASSERT_NE(doc, nullptr);

  xmlXPathContextPtr xpath_ctx = xmlXPathNewContext(doc);
  ASSERT_NE(xpath_ctx, nullptr);

  xmlXPathObjectPtr xpath_obj1 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath1.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj1, nullptr);

  xmlXPathObjectPtr xpath_obj2 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath2.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj2, nullptr);

  std::vector<std::string> tag_values;
  for (int i = 0; i < xpath_obj1->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj1->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj1->nodesetval->nodeTab[i]));
    if (content != NULL) {
      tag_values.emplace_back(content);
      xmlFree(content);
    }
  }

  std::vector<std::string> attr_values;
  for (int i = 0; i < xpath_obj2->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj2->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj2->nodesetval->nodeTab[i]));
    if (content != NULL) {
      attr_values.emplace_back(content);
      xmlFree(content);
    }
  }

  EXPECT_EQ(attr_values.size(), 0);
  EXPECT_EQ(tag_values.size(), 1);
  EXPECT_EQ(tag_values[0], "\n"
                           "    pfsense.exec_php\n"
                           "    \n"
                           "        <?php system('whoami'); ?>\n"
                           "    \n");

  xmlXPathFreeObject(xpath_obj1);
  xmlXPathFreeObject(xpath_obj2);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
}

TEST_F(XmlTest, libxml3) {
  std::string xpath1 = "/*";
  std::string xpath2 = "//@*";

  xmlDocPtr doc = xmlParseMemory(xml3_.data(), xml3_.size());
  ASSERT_NE(doc, nullptr);

  xmlXPathContextPtr xpath_ctx = xmlXPathNewContext(doc);
  ASSERT_NE(xpath_ctx, nullptr);

  xmlXPathObjectPtr xpath_obj1 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath1.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj1, nullptr);

  xmlXPathObjectPtr xpath_obj2 =
      xmlXPathEvalExpression(reinterpret_cast<const xmlChar*>(xpath2.c_str()), xpath_ctx);
  ASSERT_NE(xpath_obj2, nullptr);

  std::vector<std::string> tag_values;
  for (int i = 0; i < xpath_obj1->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj1->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj1->nodesetval->nodeTab[i]));
    if (content != NULL) {
      tag_values.emplace_back(content);
      xmlFree(content);
    }
  }

  std::vector<std::string> attr_values;
  for (int i = 0; i < xpath_obj2->nodesetval->nodeNr; ++i) {
    xmlNodePtr node = xpath_obj2->nodesetval->nodeTab[i];

    char* content;
    content = reinterpret_cast<char*>(xmlNodeGetContent(xpath_obj2->nodesetval->nodeTab[i]));
    if (content != NULL) {
      attr_values.emplace_back(content);
      xmlFree(content);
    }
  }

  EXPECT_EQ(attr_values.size(), 0);
  EXPECT_EQ(tag_values.size(), 1);
  EXPECT_EQ(tag_values[0], "\n"
                           "    pfsense.exec_php\n"
                           "    \n"
                           "        csc3232r\n"
                           R"(        exec('echo \\'  \\' > /usr/local/www/[random_file].php');)"
                           "\n"
                           "    \n");

  xmlXPathFreeObject(xpath_obj1);
  xmlXPathFreeObject(xpath_obj2);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
}

TEST_F(XmlTest, benchmark) {
  constexpr size_t test_count = 100000;

  Wge::Common::Duration duration;
  for (size_t i = 0; i < test_count; ++i) {
    Wge::Common::Ragel::Xml xml_parser;
    xml_parser.init(xml_, buffer_);
  }
  duration.stop();
  std::cout << "RAGLE XML parsing time: " << duration.milliseconds() << " ms"
            << " throughput: "
            << static_cast<double>(test_count) * xml_.size() / duration.milliseconds() * 1000 /
                   1024 / 1024 / 1024 * 8
            << " Gbps" << std::endl;

  Wge::Common::Duration duration2;
  for (size_t i = 0; i < test_count; ++i) {
    xmlDocPtr doc = xmlParseMemory(xml_.data(), xml_.size());
    xmlFreeDoc(doc);
  }
  duration2.stop();
  std::cout << "LIBXML parsing time: " << duration2.milliseconds() << " ms"
            << " throughput: "
            << static_cast<double>(test_count) * xml_.size() / duration2.milliseconds() * 1000 /
                   1024 / 1024 / 1024 * 8
            << " Gbps" << std::endl;
  // ::exit(0);
}