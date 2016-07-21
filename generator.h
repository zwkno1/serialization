#pragma once

#include "detail/objects.h"
#include "detail/parser.h"

namespace serialization
{

class generator
{
public:
    bool gen_json_code(const std::string & file, const std::string & name = "");

    bool gen_xml_code(const std::string & file, const std::string & name = "");

private:
    bool read_file(const std::string & filename, std::string & data);

    void gen_json_code(detail::tree & t, const std::string & name);

    void gen_xml_code(detail::tree & t, const std::string & name);
};

} // namespace serialization

