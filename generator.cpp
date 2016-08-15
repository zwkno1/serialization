#include "generator.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "detail/objects.h"
#include "detail/parser.h"

#include "detail/generator.h"
#include "detail/xml_generator.h"
#include "detail/json_generator.h"
#include "detail/binary_generator.h"

namespace
{

bool read_file(const std::string & filename, std::string & data)
{
    std::ifstream ifs(filename);
    if(!ifs)
    {
        return false;
    }
    ifs.seekg(0, std::ios::end);
    auto size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if(size <= 0)
    {
        return false;
    }

    data.resize(size);
    ifs.read(&data[0], size);
    return true;
}

} // namespace

namespace serialization
{

generator::generator()
{
  generators_ =
  {
      {"-xml", new serialization::detail::xml_generator},
      {"-json", new serialization::detail::json_generator},
      {"-binary", new serialization::detail::binary_generator},
  };
}

bool generator::gen_code(const std::string & type, const std::string & in, const std::string & out)
{
    auto iter = generators_.find(type);
    if(iter == generators_.end())
    {
        std::cout << "unknown code type: " << type << std::endl;
        return false;
    }

    std::string data;
    if(!read_file(in, data))
    {
        std::cout << "read file error: " << in << std::endl;
        return false;
    }

    std::string out_file = out.empty() ? in : out;

    detail::tree t;
    try
    {
        parser p(t);
        p.parse(data);
        iter->second->gen_code(t, out_file);
    }
    catch(const detail::parse_error& err)
    {
        std::cout << in << ">> " << err.where() << ":" << err.what() << std::endl;
        return false;
    }

    return true;
}

} // namespace serialization
