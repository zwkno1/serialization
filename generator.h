#pragma once

#include <string>
#include <map>

namespace serialization
{
namespace detail
{
class generator;
}
}
//class serialization::detail::generator;

namespace serialization
{

class generator
{
public:
    generator();

    bool gen_code(const std::string & type, const std::string & in, const std::string & out = "");

private:
    std::map<std::string, serialization::detail::generator *> generators_;
};

} // namespace serialization

