#include "reqbody_processor.h"

namespace SrSecurity {
namespace Variable {
const std::unordered_map<BodyProcessorType, std::string_view>
    ReqBodyProcessor::body_processor_type_map_{{BodyProcessorType::UrlEncoded, "URLENCODED"},
                                               {BodyProcessorType::MultiPart, "MULTIPART"},
                                               {BodyProcessorType::Xml, "XML"},
                                               {BodyProcessorType::Json, "JSON"}};
} // namespace Variable
} // namespace SrSecurity