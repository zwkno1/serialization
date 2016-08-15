#include "json_generator.h"

#include <fstream>
#include <sstream>

namespace serialization
{

namespace detail
{

void json_generator::gen_code(const serialization::detail::tree & t, const std::string & name)
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
               "bool serialize(rapidjson::Value & , const " << t.type << "::" << e->type << " & , rapidjson::Document & );\n"
            << "template<>\n"
               "bool unserialize(const rapidjson::Value & , " << t.type << "::" << e->type << " & );\n\n";

        os2 << "template<>\n"
               "bool serialize(rapidjson::Value & val, const " << t.type << "::" << e->type << " & in, rapidjson::Document & doc)\n"
               "{\n"
               "rapidjson::Value v(rapidjson::kObjectType);\n";

        os3 << "template<>\n"
               "bool unserialize(const rapidjson::Value & val, " << t.type << "::" << e->type << " & out)\n"
               "{\n"
               "if(!val.IsObject())\n"
               "return false;\n\n";

        for(serialization::detail::item * i : e->items)
        {
            os2 << "{\n"
                   "rapidjson::Value v1;\n"
                   "if(!serialize(v1, in." << i->name << ", doc))\n"
                   "return false;\n"
                   "v.AddMember(\"" << i->name << "\", std::move(v1), doc.GetAllocator());\n"
                   "}\n";

            os3 << "if(!unserialize(val[\"" << i->name << "\"], out." << i->name << "))\n"
                   "return false;\n";
        }

        os2 << "val = std::move(v);\n"
               "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    h << "\n"
         "#pragma once\n"
         "#include <list>\n"
         "#include <vector>\n"
         "#include <string>\n"
         "#include <sstream>\n"
         "\n"
         "#include <rapidjson/rapidjson.h>\n"
         "#include <rapidjson/document.h>\n"
         "\n"
         "\n";

    gen_struct_def(t, h);
    h << "#include \"json_base.h\"\n\n";
    h << os1.rdbuf();

    cpp << "#include \"" << name << ".h"
           "\"\n\n"
           "#include <cstring>\n"
           "\n";
    cpp << os2.rdbuf();
    cpp << os3.rdbuf();
}

} //namespace detail

} //namespace serialization
