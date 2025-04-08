#include "xml.h"

#include <xml.h>

namespace SrSecurity {
namespace Common {
namespace Ragel {
void Xml::init(std::string_view xml_str) {
  parseXml(xml_str, attr_values_, tag_values_);
}
} // namespace Ragel
} // namespace Common
} // namespace SrSecurity
