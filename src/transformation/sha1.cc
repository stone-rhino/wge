#include "sha1.h"

#include <iostream>

#include <boost/algorithm/hex.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>

namespace SrSecurity {
namespace Transformation {
std::string Sha1::evaluate(const void* data, size_t data_len) const {
  std::string result;

  // Check the input
  if (data == nullptr || data_len == 0) [[unlikely]] {
    return result;
  }

  // Process the data
  boost::uuids::detail::sha1 sha1_;
  sha1_.process_bytes(data, data_len);
  boost::uuids::detail::sha1::digest_type digest;
  sha1_.get_digest(digest);

  // Convert the digest to network byte order
  boost::uuids::detail::sha1::digest_type order;
  memcpy(&order, &digest, sizeof(order));
  for (size_t i = 0; i < sizeof(digest) / sizeof(int); ++i) {
    for (size_t j = 0; j < sizeof(int); ++j) {
      reinterpret_cast<char*>(&order[i])[j] =
          reinterpret_cast<char*>(&digest[i])[sizeof(int) - j - 1];
    }
  }

  // Copy the digest to the result
  const auto char_digest = reinterpret_cast<const char*>(&order);
  result.append(char_digest, sizeof(boost::uuids::detail::sha1::digest_type));
  return result;
}
} // namespace Transformation
} // namespace SrSecurity
