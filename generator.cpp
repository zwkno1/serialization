#include "generator.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "detail/objects.h"
#include "detail/parser.h"

namespace
{

const char * base_code =
        R"(

        // for base item
        // char or uchar is treated as interger"
        template<class T>
        bool serialize(rapidjson::Value & val, const T & in, rapidjson::Document & doc)
        {
        rapidjson::Value v(rapidjson::kStringType);
        std::stringstream ss;
        ss << in;
        v.SetString(ss.str().c_str(), doc.GetAllocator());
        val = std::move(v);
        return true;
        }

        // for sequence
        template<class T>
        bool serialize(rapidjson::Value & val, const std::list<T> & in, rapidjson::Document & doc)
        {
        rapidjson::Value v(rapidjson::kArrayType);
        for(auto &i : in)
        {
        rapidjson::Value v1;
        if(!serialize(v1, i, doc))
        return false;
        v.PushBack(std::move(v1), doc.GetAllocator());
        }

        val = std::move(v);

        return true;
        }

        // for base item
        template<class T>
        bool unserialize(const rapidjson::Value & val, T & out)
        {
        if(!val.IsString())
        return false;
        std::stringstream ss;
        ss << val.GetString();
        ss >> out;
        if(!ss.eof())
        return false;
        return true;
        }

        // for sequence
        template<class T>
        bool unserialize(const rapidjson::Value & val, std::list<T> & out)
        {
        if(!val.IsArray())
        return false;
        T tmp;
        for(auto i = val.Begin(); i != val.End(); ++i)
        {
        if(!unserialize(*i, tmp))
        return false;
        out.push_back(std::move(tmp));
        }
        return true;
        }

        )";

} // namespace


namespace serialization
{

bool generator::gen_json_code(const std::string & file, const std::string & name)
{
    std::string data;
    if(!read_file(file, data))
    {
        std::cout << "read file error: " << file << std::endl;
        return false;
    }

    detail::tree t;
    try
    {
        parser p(t);
        p.parse(data);
        if(name.empty())
            gen_json_code(t, file);
        else
            gen_json_code(t, name);
    }
    catch(const detail::parse_error& err)
    {
        std::cout << file << ">> " << err.where() << ":" << err.what() << std::endl;
        return false;
    }

    return true;
}


bool generator::read_file(const std::string & filename, std::string & data)
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

void generator::gen_json_code(detail::tree & t, const std::string & name)
{
    std::ofstream h(name + ".h");
    std::ofstream cpp(name + ".cpp");

    //function def code
    std::stringstream os1;
    //serialize code
    std::stringstream os2;
    //unserialize code
    std::stringstream os3;

    h << "\n"
         "#pragma once\n"
         "#include <list>\n"
         "#include <string>\n"
         "#include <sstream>\n"
         "\n"
         "#include <rapidjson/rapidjson.h>\n"
         "#include <rapidjson/document.h>\n"
         "\n"
         "\n";

    cpp << "#include \"" << name << ".h"
           "\"\n\n"
           "#include <cstring>\n"
           "\n";

    if(!t.desc.empty())
        h << "// " << t.desc << "\n";

    if(!t.type.empty())
        h << "namespace " << t.type << "\n{\n";

    for(detail::entry * e : t.entries)
    {
        h << "// " << e->desc << "\n";
        h << "struct " << e->type << "\n{\n";
        if(!e->id.empty())
            h << "enum { ID = " << e->id << " };\n\n";

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

        for(detail::item * i : e->items)
        {
            h << "// " << i->desc << "\n";
            if(i->item_type == detail::Sequence)
                h << "std::list< " << i->type << " >    " << i->name << ";\n\n";
            else
                h << i->type << "    " << i->name << ";\n\n";

            os2 << "{\n"
                   "rapidjson::Value v1;\n"
                   "if(!serialize(v1, in." << i->name << ", doc))\n"
                                                         "return false;\n"
                                                         "v.AddMember(\"" << i->name << "\", std::move(v1), doc.GetAllocator());\n"
                                                                                        "}\n";

            os3 << "if(!unserialize(val[\"" << i->name << "\"], out." << i->name << "))\n"
                                                                                    "return false;\n";
        }

        h << "};\n\n";

        os2 << "val = std::move(v);\n"
               "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    if(!t.type.empty())
        h << "\n} // namespace " << t.type << "\n";
    h << "\n\n";
    h << base_code;
    h << os1.rdbuf();

    cpp << os2.rdbuf();
    cpp << os3.rdbuf();
}

} // namespace serialization
