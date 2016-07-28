#include "generator.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "detail/objects.h"
#include "detail/parser.h"

namespace
{

const char * xml_base_code =
        R"(
        // for base item
        // char or uchar is treated as interger"
        template<class T>
        bool serialize(rapidxml::xml_node<> * node, const T & in, rapidxml::xml_document<> & doc)
        {
        std::stringstream ss;
        ss << in;
        node->value(doc.allocate_string(ss.str().c_str()));
        return true;
        }

        // for sequence
        template<class T>
        bool serialize(rapidxml::xml_node<> * node, const std::list<T> & in, rapidxml::xml_document<> & doc)
        {
        for(auto & i : in)
        {
        auto node1 = doc.allocate_node(rapidxml::node_element, "item");
        node->append_node(node1);
        if(!serialize(node1, i, doc))
        return false;
        }
        return true;
        }

        template<class T>
        bool unserialize(const rapidxml::xml_node<> * node, T & out)
        {
        if(!node)
        return false;
        std::stringstream ss;
        ss << node->value();
        ss >> out;
        return ss.eof();
        }

        template<class T>
        bool unserialize(const rapidxml::xml_node<> * node, std::list<T> & out)
        {
        if(!node)
        return false;
        for(auto i = node->first_node(); i; i = i->next_sibling())
        {
        if(std::strcmp(i->name(), "item"))
        return false;
        T tmp;
        if(!unserialize(i, tmp))
        return false;
        out.push_back(std::move(tmp));
        }
            return true;
        }
)";


const char * json_base_code =
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

const char * binary_base_code =
        R"(
        // for base item
        template<typename T>
        bool serialize(Buffer & buf, const T & in)
        {
        buf.append(in);
        return true;
        }

        // for sequence
        template<typename T>
        bool serialize(Buffer & buf, const std::list<T> & in)
        {
        uint32_t size = in.size();
        buf.append(size);
        for(auto &i : in)
        {
        if(!serialize(buf, i))
        return false;
        }
        return true;
        }

        // for base item
        template<typename T>
        bool unserialize(const Buffer & buf, T & out)
        {
        return buf.get(out);
        }

        // for sequence
        template<typename T>
        bool unserialize(const Buffer & buf, std::list<T> & out)
        {
        uint32_t size;
        if(!buf.get(size))
        return false;

        T tmp;
        for(uint32_t i = 0; i != size; ++i)
        {
        if(!unserialize(buf, tmp))
        return false;
        }
        return true;
        }
        )";

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

void gen_json_code_impl(serialization::detail::tree & t, const std::string & name)
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

    for(serialization::detail::entry * e : t.entries)
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

        for(serialization::detail::item * i : e->items)
        {
            h << "// " << i->desc << "\n";
            if(i->item_type == serialization::detail::Sequence)
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
    h << json_base_code;
    h << os1.rdbuf();

    cpp << os2.rdbuf();
    cpp << os3.rdbuf();
}

void gen_xml_code_impl(serialization::detail::tree & t, const std::string & name)
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
         "#include <cstring>\n"
         "#include <sstream>\n"
         "\n"
         "#include <rapidxml/rapidxml.hpp>\n"
         "\n"
         "\n";

    cpp << "#include \"" << name << ".h\"\n"
           "\n";

	h << "#pragma pack(push,1)\n\n";
    if(!t.desc.empty())
        h << "// " << t.desc << "\n";

    if(!t.type.empty())
        h << "namespace " << t.type << "\n{\n";

    for(serialization::detail::entry * e : t.entries)
    {
        h << "// " << e->desc << "\n";
        h << "struct " << e->type << "\n{\n";
        if(!e->id.empty())
            h << "enum { ID = " << e->id << " };\n\n";

        os1 << "template<>\n"
               "bool serialize(rapidxml::xml_node<> * , const " << t.type << "::" << e->type << " & , rapidxml::xml_document<> & );\n"
            << "template<>\n"
               "bool unserialize(const rapidxml::xml_node<> *, " << t.type << "::" << e->type << " & );\n\n";

        os2 << "template<>\n"
               "bool serialize(rapidxml::xml_node<> * node, const " << t.type << "::" << e->type << " & in, rapidxml::xml_document<> & doc)\n"
               "{\n";

        os3 << "template<>\n"
               "bool unserialize(const rapidxml::xml_node<> * node, " << t.type << "::" << e->type << " & out)\n"
               "{\n"
               "if(!node)\n"
               "return false;"
               "node = node->first_node();\n\n";

        for(serialization::detail::item * i : e->items)
        {
            h << "// " << i->desc << "\n";
            if(i->item_type == serialization::detail::Sequence)
                h << "std::list< " << i->type << " >    " << i->name << ";\n\n";
            else
                h << i->type << "    " << i->name << ";\n\n";

            os2 << "{\n"
                   "auto node1 = doc.allocate_node(rapidxml::node_element, \"" << i->name << "\");\n"
                   "node->append_node(node1);\n"
                   "if(!serialize(node1, in." << i->name << ", doc))\n"
                   "return false;\n"
                   "}\n";

            os3 << "if(!node || std::strcmp(node->name(), \"" << i->name << "\"))\n"
                   "return false;\n"
                   "if(!unserialize(node, out." << i->name << "))\n"
                   "return false;\n"
                   "node = node->next_sibling();\n"
                   "\n";
        }

        h << "};\n\n";

        os2 << "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    if(!t.type.empty())
        h << "\n} // namespace " << t.type << "\n";
    h << "\n\n";
	h << "#pragma pack(pop)\n\n";
    h << xml_base_code;
    h << os1.rdbuf();

    cpp << os2.rdbuf();
    cpp << os3.rdbuf();
}

void gen_binary_code_impl(serialization::detail::tree & t, const std::string & name)
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
         "#include <cstring>\n"
         "#include <sstream>\n"
         "\n"
		 "#include \"buffer.h\"\n"
         "\n"
         "\n";

    cpp << "#include \"" << name << ".h\"\n"
           "\n";

    h << "#pragma pack(push,1)\n\n";

    if(!t.desc.empty())
        h << "// " << t.desc << "\n";

    if(!t.type.empty())
        h << "namespace " << t.type << "\n{\n";

    for(serialization::detail::entry * e : t.entries)
    {
        h << "// " << e->desc << "\n";
        h << "struct " << e->type << "\n{\n";
        if(!e->id.empty())
            h << "enum { ID = " << e->id << " };\n\n";

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
            h << "// " << i->desc << "\n";
            if(i->item_type == serialization::detail::Sequence)
                h << "std::list< " << i->type << " >    " << i->name << ";\n\n";
            else
                h << i->type << "    " << i->name << ";\n\n";

            os2 << "if(!serialize(buf, in." << i->name << "))\n"
                   "return false;\n"
                   "\n";

            os3 << "if(!unserialize(buf, out." << i->name << "))\n"
                   "return false;\n"
                   "\n";
        }

        h << "};\n\n";

        os2 << "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    if(!t.type.empty())
        h << "\n} // namespace " << t.type << "\n";
    h << "\n\n";
    h << "#pragma pack(pop)\n\n";
    h << binary_base_code;
    h << os1.rdbuf();

    cpp << os2.rdbuf();
    cpp << os3.rdbuf();

}

} // namespace

namespace serialization
{

bool generator::gen_code(CodeType type, const std::string & in, const std::string & out)
{
    std::string data;
    if(!read_file(in, data))
    {
        std::cout << "read file error: " << in << std::endl;
        return false;
    }

    std::string filename = out.empty() ? in : out;

    detail::tree t;
    try
    {
        parser p(t);
        p.parse(data);
        switch(type)
        {
        case JsonCodeType:
        {
            gen_json_code_impl(t, filename);
        }
            break;
        case XmlCodeType:
        {
            gen_xml_code_impl(t, filename);
        }
            break;
        case BinaryCodeType:
        {
            gen_binary_code_impl(t, filename);
        }
            break;
        default:
            return false;
        }
    }
    catch(const detail::parse_error& err)
    {
        std::cout << in << ">> " << err.where() << ":" << err.what() << std::endl;
        return false;
    }

    return true;
}

bool generator::gen_json_code(const std::string & in, const std::string & out)
{
    return gen_code(JsonCodeType, in, out);
}

bool generator::gen_xml_code(const std::string & in, const std::string & out)
{
    return gen_code(XmlCodeType, in, out);
}

bool generator::gen_binary_code(const std::string & in, const std::string & out)
{
    return gen_code(BinaryCodeType, in, out);
}

} // namespace serialization
