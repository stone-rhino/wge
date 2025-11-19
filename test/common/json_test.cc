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

#include "common/duration.h"
#include "common/ragel/json.h"

class JsonTest : public ::testing::Test {
protected:
  static constexpr std::string_view json_ = R"({
    "na\"me": "Trump\a \b \f \n \r \t \v \\ \? \' \" \xab \101 \01 \1",
    "age": 18,
    "isStudent": true,
    "weight": 60.5,
    "height": 1.75,
    "family": {
        "father": "Trump Sr.",
        "mother": "Jane",
        "sibling": "Jack"
    },
    "array_strings": [
       "string1",
       "string2",
       "string3" 
    ],
    "array_numbers": [
        1,
        2,
        3
    ],
    "array_booleans": [
        true,
        false,
        true
    ],
    "array_floats": [
      [
        [1.1, 2.2, 3.3], [4.4, 5.5, 6.6]
      ],
      [
        [1.1, 2.2, 3.3], [4.4, 5.5, 6.6]
      ]
    ],
    "array_objects": [
        {
            "name": "name1",
            "type": "type1",
            "value": [
                1,
                2,
                3
            ]
        },
        {
            "name": "name2",
            "type": "type2",
            "value": [
                4,
                5,
                6
            ]
        }
    ]
})";

  std::forward_list<std::string> buffer_;
};

TEST_F(JsonTest, BlockParse) {
  Wge::Common::Ragel::Json json_parser;
  json_parser.init(json_, buffer_);
  auto& key_values_map = json_parser.getKeyValues();
  auto& key_values_linked = json_parser.getKeyValuesLinked();
  EXPECT_EQ(key_values_map.size(), 23);
  EXPECT_EQ(key_values_linked.size(), 23);

  EXPECT_EQ(key_values_linked[0].first, "na\"me");
  EXPECT_EQ(key_values_linked[0].second, "Trump\a \b \f \n \r \t \v \\ \? \' \" \xab A \1 \1");

  EXPECT_EQ(key_values_linked[1].first, "age");
  EXPECT_EQ(key_values_linked[1].second, "");

  EXPECT_EQ(key_values_linked[2].first, "isStudent");
  EXPECT_EQ(key_values_linked[2].second, "");

  EXPECT_EQ(key_values_linked[3].first, "weight");
  EXPECT_EQ(key_values_linked[3].second, "");

  EXPECT_EQ(key_values_linked[4].first, "height");
  EXPECT_EQ(key_values_linked[4].second, "");

  EXPECT_EQ(key_values_linked[5].first, "family");
  EXPECT_EQ(key_values_linked[5].second, "");

  EXPECT_EQ(key_values_linked[6].first, "father");
  EXPECT_EQ(key_values_linked[6].second, "Trump Sr.");

  EXPECT_EQ(key_values_linked[7].first, "mother");
  EXPECT_EQ(key_values_linked[7].second, "Jane");

  EXPECT_EQ(key_values_linked[8].first, "sibling");
  EXPECT_EQ(key_values_linked[8].second, "Jack");

  EXPECT_EQ(key_values_linked[9].first, "array_strings");
  EXPECT_EQ(key_values_linked[9].second, "");

  EXPECT_EQ(key_values_linked[10].first, "");
  EXPECT_EQ(key_values_linked[10].second, "string1");

  EXPECT_EQ(key_values_linked[11].first, "");
  EXPECT_EQ(key_values_linked[11].second, "string2");

  EXPECT_EQ(key_values_linked[12].first, "");
  EXPECT_EQ(key_values_linked[12].second, "string3");

  EXPECT_EQ(key_values_linked[13].first, "array_numbers");
  EXPECT_EQ(key_values_linked[13].second, "");

  EXPECT_EQ(key_values_linked[14].first, "array_booleans");
  EXPECT_EQ(key_values_linked[14].second, "");

  EXPECT_EQ(key_values_linked[15].first, "array_floats");
  EXPECT_EQ(key_values_linked[15].second, "");

  EXPECT_EQ(key_values_linked[16].first, "array_objects");
  EXPECT_EQ(key_values_linked[16].second, "");

  EXPECT_EQ(key_values_linked[17].first, "name");
  EXPECT_EQ(key_values_linked[17].second, "name1");

  EXPECT_EQ(key_values_linked[18].first, "type");
  EXPECT_EQ(key_values_linked[18].second, "type1");

  EXPECT_EQ(key_values_linked[19].first, "value");
  EXPECT_EQ(key_values_linked[19].second, "");

  EXPECT_EQ(key_values_linked[20].first, "name");
  EXPECT_EQ(key_values_linked[20].second, "name2");

  EXPECT_EQ(key_values_linked[21].first, "type");
  EXPECT_EQ(key_values_linked[21].second, "type2");

  EXPECT_EQ(key_values_linked[22].first, "value");
  EXPECT_EQ(key_values_linked[22].second, "");
}

TEST_F(JsonTest, StreamParse) {
  // Use the same json string as in the block parse for baseline comparison
  Wge::Common::Ragel::Json json_parser;
  json_parser.init(json_, buffer_);
  auto& key_values_linked = json_parser.getKeyValuesLinked();

  // Construct the baseline key-value pairs
  std::string key_strings;
  std::string value_strings;
  for (auto [key, value] : key_values_linked) {
    key_strings += key;
    value_strings += value;
  }

  // Test the stream parsing with different step sizes
  for (size_t step = 1; step <= 50; step++) {
    auto state = Wge::Common::Ragel::Json::newStream();
    std::string test_key_strings;
    std::string test_value_strings;
    for (size_t j = 0; j < json_.size();) {
      size_t input_step = std::min(step, json_.size() - j);
      std::string_view input(&json_[j], input_step);
      std::unordered_multimap<std::string_view, std::string_view> key_value_map;
      std::list<Wge::Common::Ragel::KeyValuePair> key_value_linked;
      auto stream_result = Wge::Common::Ragel::Json::parseStream(
          input, key_value_map, key_value_linked, *state, j + input_step >= json_.size());
      EXPECT_NE(stream_result, Wge::Transformation::StreamResult::INVALID_INPUT);
      j += input_step;

      for (auto& kv : key_value_linked) {
        test_key_strings += kv.key_;
        test_value_strings += kv.value_;
      }
    }
    EXPECT_EQ(test_key_strings, key_strings);
    EXPECT_EQ(test_value_strings, value_strings);
  }
}

TEST_F(JsonTest, benchmark) {
  constexpr size_t test_count = 100000;

  Wge::Common::Duration duration;
  for (size_t i = 0; i < test_count; ++i) {
    Wge::Common::Ragel::Json json_parser;
    json_parser.init(json_, buffer_);
  }
  duration.stop();
  std::cout << "RAGLE Json parsing time: " << duration.milliseconds() << " ms"
            << " throughput: "
            << static_cast<double>(test_count) * json_.size() / duration.milliseconds() * 1000 /
                   1024 / 1024 / 1024 * 8
            << " Gbps" << std::endl;
  // ::exit(0);
}