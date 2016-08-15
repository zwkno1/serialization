#include "binary_generator.h"

#include <fstream>
#include <sstream>

namespace serialization
{

namespace detail
{

void binary_generator::gen_code(const serialization::detail::tree & t, const std::string & name)
{
    std::ofstream h(name + ".h");
    std::ofstream cpp(name + ".cpp");

    //function def code
    std::stringstream os1;
    //serialize code
    std::stringstream os2;
    //unserialize code
    std::stringstream os3;

    for(serialization::detail::entry * e : t.entries)
    {
        os1 << "template<>\n"
               "bool serialize(Buffer & , const " << t.type << "::" << e->type << " & );\n"
            << "template<>\n"
               "bool unserialize(const Buffer & , " << t.type << "::" << e->type << " & );\n\n";

        os2 << "template<>\n"
               "bool serialize(Buffer & buf, const " << t.type << "::" << e->type << " & in)\n"
               "{\n";

        os3 << "template<>\n"
               "bool unserialize(const Buffer & buf, " << t.type << "::" << e->type << " & out)\n"
               "{\n";

        for(serialization::detail::item * i : e->items)
        {
            os2 << "if(!serialize(buf, in." << i->name << "))\n"
                   "return false;\n"
                   "\n";

            os3 << "if(!unserialize(buf, out." << i->name << "))\n"
                   "return false;\n"
                   "\n";
        }

        os2 << "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    h << "\n"
         "#pragma once\n"
         "#include <list>\n"
         "#include <vector>\n"
         "#include <string>\n"
         "#include <cstring>\n"
         "#include <sstream>\n"
         "\n"
         "#include \"buffer.h\"\n"
         "\n"
         "\n";
    h << "#pragma pack(push,1)\n\n";
    gen_struct_def(t, h);
    h << "#pragma pack(pop)\n\n";
    h << "#include \"binary_base.h\"\n\n";
    h << os1.rdbuf();

    cpp << "#include \"" << name << ".h\"\n"
           "\n";
    cpp << os2.rdbuf();
    cpp << os3.rdbuf();

}

} //namespace detail

} //namespace serialization
