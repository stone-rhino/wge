#include "html_entity_decode.h"

namespace SrSecurity {
namespace Transformation {
const std::array<std::pair<std::string, char>, 6> HtmlEntityDecode::html_entitys_{
    {{"&quot;", '\"'},
     {"&amp;", '&'},
     {"&lt;", '<'},
     {"&gt;", '>'},
     {"&apos;", '\''},
     {"&nbsp;", ' '}}};
std::regex HtmlEntityDecode::html_entity_regex_("(&quot;)|(&amp;)|(&lt;)|(&gt;)|(&apos;)|(&nbsp;)");
} // namespace Transformation
} // namespace SrSecurity
