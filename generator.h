#pragma once

#include <string>

namespace serialization
{

class generator
{
public:
    enum CodeType
    {
        XmlCodeType = 0,
        JsonCodeType,
        BinaryCodeType,
    };

    bool gen_code(CodeType type, const std::string & in, const std::string & out = "");

    bool gen_json_code(const std::string & in, const std::string & out = "");

    bool gen_xml_code(const std::string & in, const std::string & out = "");

    bool gen_binary_code(const std::string & in, const std::string & out = "");

};

} // namespace serialization

