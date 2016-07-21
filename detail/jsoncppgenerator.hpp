#pragma once

#include "objects.hpp"
#include <memory>
#include <iostream>

namespace serialization
{

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

}

namespace
{

void genDefinitionCommon(std::ostream & os, const std::string & type, const std::string & name, const std::string & desc)
{
    os << "// " << desc << "\n";
    os << type << "    " << name << ";\n\n";
}

void genUnserializeCommon(std::ostream & os, const std::string & name)
{
    os << "if(!unserialize(val[\"" << name << "\"], out." << name << "))\n"
          "return false;\n";
}

void genSerializeCommon(std::ostream & os, const std::string & name)
{
    os << "{\n"
          "rapidjson::Value v1;\n"
          "if(!serialize(v1, in." << name << ", doc))\n"
          "return false;\n"
          "v.AddMember(\"" << name << "\", std::move(v1), doc.GetAllocator());\n"
          "}\n";
}

} // namespace

class json_cpp_base : public detail::base
{
public:
    json_cpp_base(const std::string & type, const std::string & name, const std::string & desc)
        : base(type, name, desc)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genDefinitionCommon(os, type_, name_, desc_);
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genUnserializeCommon(os, name_);
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genSerializeCommon(os, name_);
    }
};

// Concept requirements:
// sequence::value_type
// sequence::push_back
class json_cpp_sequence: public detail::sequence
{
public:
    json_cpp_sequence(const std::string & type, const std::string & name, const std::string & desc)
        : sequence(type, name, desc)
        , impl_type_("std::list< " + type + " >")
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, impl_type_);

        genDefinitionCommon(os, impl_type_, name_, desc_);
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, impl_type_);

        genUnserializeCommon(os, name_);
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genSerializeCommon(os, name_);
    }

private:
    std::string impl_type_;
};

class json_cpp_structure : public detail::structure
{
public:
    json_cpp_structure(const std::string & type, const std::string & name, const std::string & desc)
        : structure(type, name, desc)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genDefinitionCommon(os, type_, name_, desc_);
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genUnserializeCommon(os, name_);
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        genSerializeCommon(os, name_);
    }
};

class json_cpp_entry : public detail::entry
{
public:
    json_cpp_entry(const std::string & ns, const std::string & type, const std::string & desc, const std::string & id)
        : entry(ns, type, desc, id)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << "// " << desc_ << "\n";
        os << "struct " << type_ << "\n{\n";

        if(!id_.empty())
        {
            os << "enum { ID = " << id_ << " };\n\n";
        }

        for(auto i : items_)
        {
            i->genDefinition(os);
        }

        os << "};\n\n";
    }

    void genFuncDef(std::ostream & os) override
    {
        os << "template<>\n"
              "bool serialize(rapidjson::Value & , const " << namespace_ << "::" << type_ << " & , rapidjson::Document & );\n"
           << "template<>\n"
              "bool unserialize(const rapidjson::Value & , " << namespace_ << "::" << type_ << " & );\n\n";
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << "template<>\n"
              "bool unserialize(const rapidjson::Value & val, " << namespace_ << "::" << type_ << " & out)\n"
              "{\n"
              "if(val.IsNull())\n"
              "return false;\n\n";

        for(auto i : items_)
        {
            i->genUnserialize(os);
        }

        os << "\nreturn true;\n"
              "}\n\n";
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << "template<>\n"
              "bool serialize(rapidjson::Value & val, const " << namespace_ << "::" << type_ << " & in, rapidjson::Document & doc)\n"
              "{\n"
              "rapidjson::Value v(rapidjson::kObjectType);\n";
        for(auto i : items_)
        {
            i->genSerialize(os);
        }
        os << "val = std::move(v);\n"
              "return true;\n"
              "}\n\n";
    }
};

class json_cpp_root : public detail::root
{
public:
    json_cpp_root(const std::string & type, const std::string & desc)
        : root(type, desc)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        if(!type_.empty())
            os << "namespace " << type_ << "\n"
                                           "{\n";

        for(auto i : entries_)
        {
            i->genDefinition(os);
        }

        if(!type_.empty())
            os << "} // namespace " << type_ << "\n"
                                                "\n";
    }

    void genFuncDef(std::ostream &os)
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        for(auto i : entries_)
        {
            i->genFuncDef(os);
        }
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        for(auto i : entries_)
        {
            i->genUnserialize(os);
        }
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        for(auto i : entries_)
        {
            i->genSerialize(os);
        }
    }

    void genBaseFunc(std::ostream & os)
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << base_code ;

    }
};

struct json_cpp_object_factory
{
public:
    typedef detail::item * item_type;
    typedef json_cpp_entry * entry_type;
    typedef json_cpp_root * root_type;

    item_type make_base(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new json_cpp_base(type, name, desc);
    }

    item_type make_sequence(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new json_cpp_sequence(type, name, desc);
    }

    item_type make_structure(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new json_cpp_structure(type, name, desc);
    }

    entry_type make_entry(const std::string & ns, const std::string & type, const std::string & desc, const std::string & id)
    {
        return new json_cpp_entry(ns, type, desc, id);
    }

    root_type make_root(const std::string & type, const std::string & desc)
    {
        return new json_cpp_root(type, desc);
    }

    root_type root;
};

template<template<typename ObjectFactory> class Parser>
class json_cpp_generator
{
    typedef Parser<json_cpp_object_factory> parser;
    typedef json_cpp_object_factory factory;
public:
    bool gen(const std::string & file)
    {
        std::ifstream ifs(file);
        if(!ifs)
        {
            std::cout << "file not found: " << file;
            return false;
        }
        ifs.seekg(0, std::ios::end);
        auto size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        if(size <= 0)
        {
            std::cout << "file size error: " << file;
            return false;
        }

        std::string xml;
        xml.resize(size);
        ifs.read(&xml[0], size);

        json_cpp_object_factory factory;
        try
        {
            parser p(factory);
            p.parse(xml);
        }
        catch(const serialization::parse_error& err)
        {
            std::cout << file << ">> " << err.where() << ":" << err.what() << std::endl;
            return false;
        }

        std::ofstream head(file + ".h");
        std::ofstream cpp(file + ".cpp");

        head << "\n"
                "#pragma once\n"
                "#include <list>\n"
                "#include <string>\n"
                "#include <sstream>\n"
                "\n"
                "#include <rapidjson/rapidjson.h>\n"
                "#include <rapidjson/document.h>\n"
                "\n"
                "\n";

        factory.root->genDefinition(head);
        factory.root->genBaseFunc(head);
        factory.root->genFuncDef(head);

        cpp << "#include \"" << file << ".h" << "\"\n";
        cpp << "\n"
               "#include <cstring>\n"
               "\n";

        factory.root->genSerialize(cpp);
        factory.root->genUnserialize(cpp);
        return true;
    }
};
} // namespace serialization

