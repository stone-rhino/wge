#pragma once

#include <memory>
#include <string>

#define SHA1_VALUE_SIZE 20

namespace boost {
namespace uuids {
namespace detail {
class sha1;
}
} // namespace uuids
} // namespace boost

namespace Wge {
namespace Common {
class Sha1 {
public:
  Sha1();
  ~Sha1(){};

public:
  void update(const void* data, size_t size);
  void update(const std::string& data);
  std::string update(const void* data, size_t size, bool hex_encode);
  std::string update(const std::string& data, bool hex_encode);

public:
  static std::string marshal(const void* data, size_t size, bool hex_encode);
  static std::string marshal(const std::string& data, bool hex_encode);

private:
  static void updateInternal(const void* data, size_t size,
                             std::shared_ptr<boost::uuids::detail::sha1> sha1);
  static std::string updateInternal(const void* data, size_t size,
                                    std::shared_ptr<boost::uuids::detail::sha1> sha1,
                                    bool hex_encode);
  std::shared_ptr<boost::uuids::detail::sha1> sha1_;
};
} // namespace Common
} // namespace Wge