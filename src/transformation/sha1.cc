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
#include "sha1.h"

#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>

namespace Wge {
namespace Transformation {
bool Sha1::evaluate(std::string_view data, std::string& result) const {
  result.clear();

  // Process the data
  boost::uuids::detail::sha1 sha1;
  sha1.process_bytes(data.data(), data.length());
  boost::uuids::detail::sha1::digest_type digest;
  sha1.get_digest(digest);

  // Convert the digest to network byte order
  boost::uuids::detail::sha1::digest_type order;
  memcpy(&order, &digest, sizeof(order));
  // for (size_t i = 0; i < sizeof(digest) / sizeof(int); ++i) {
  //   for (size_t j = 0; j < sizeof(int); ++j) {
  //     reinterpret_cast<char*>(&order[i])[j] =
  //         reinterpret_cast<char*>(&digest[i])[sizeof(int) - j - 1];
  //   }
  // }

  // Copy the digest to the result
  const auto char_digest = reinterpret_cast<const char*>(&order);
  boost::algorithm::hex_lower(char_digest,
                              char_digest + sizeof(boost::uuids::detail::sha1::digest_type),
                              std::back_inserter(result));
  return true;
}

std::unique_ptr<StreamState, std::function<void(StreamState*)>> Sha1::newStream() const {
  auto state = std::unique_ptr<StreamState, std::function<void(StreamState*)>>(
      new StreamState(), [](StreamState* state) {
        boost::uuids::detail::sha1* sha1 =
            reinterpret_cast<boost::uuids::detail::sha1*>(state->extra_state_buffer_.data());
        sha1->~sha1();
        delete state;
      });

  state->extra_state_buffer_.resize(sizeof(boost::uuids::detail::sha1));
  boost::uuids::detail::sha1* sha1 =
      reinterpret_cast<boost::uuids::detail::sha1*>(state->extra_state_buffer_.data());
  new (sha1) boost::uuids::detail::sha1();

  return state;
}

StreamResult Sha1::evaluateStream(std::string_view input, std::string& output, StreamState& state,
                                  bool end_stream) const {
  boost::uuids::detail::sha1* sha1 =
      reinterpret_cast<boost::uuids::detail::sha1*>(state.extra_state_buffer_.data());

  std::string_view input_data = input;
  sha1->process_bytes(input_data.data(), input_data.length());

  if (end_stream) {
    boost::uuids::detail::sha1::digest_type digest;
    sha1->get_digest(digest);

    // Convert the digest to network byte order
    boost::uuids::detail::sha1::digest_type order;
    memcpy(&order, &digest, sizeof(order));
    // for (size_t i = 0; i < sizeof(digest) / sizeof(int); ++i) {
    //   for (size_t j = 0; j < sizeof(int); ++j) {
    //     reinterpret_cast<char*>(&order[i])[j] =
    //         reinterpret_cast<char*>(&digest[i])[sizeof(int) - j - 1];
    //   }
    // }

    // Copy the digest to the result
    const auto char_digest = reinterpret_cast<const char*>(&order);
    output.clear();
    boost::algorithm::hex_lower(char_digest,
                                char_digest + sizeof(boost::uuids::detail::sha1::digest_type),
                                std::back_inserter(output));

    return StreamResult::SUCCESS;
  } else {
    return StreamResult::NEED_MORE_DATA;
  }
}
} // namespace Transformation
} // namespace Wge
