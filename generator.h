#pragma once

#include <string>

namespace serialization
{

class generator
{
public:
    bool gen_json_code(const std::string & file, const std::string & name = "");

    bool gen_xml_code(const std::string & file, const std::string & name = "");
};

} // namespace serialization

