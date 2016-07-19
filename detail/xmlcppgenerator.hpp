#pragma once

#include "objects.hpp"
#include <memory>
#include <iostream>

namespace serialization
{

namespace
{
const char * const xml_node_type_str = "rapidxml::xml_node<> *";
const char * const xml_document_type_str =  "rapidxml::xml_document<>";
}

class xml_cpp_base : public detail::base
{
public:
    xml_cpp_base(const std::string & type, const std::string & name, const std::string & desc)
        : base(type, name, desc)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "// " << desc_ << "\n";
        os << type_ << "    " << name_ << ";\n\n";
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "if(!node || std::strcmp(node->name(), \"" << name_ << "\"))\n"
              "return false;\n"
              "if(!unserialize(node, out." << name_ << "))\n"
              "return false;\n"
              "node = node->next_sibling();\n"
              "\n";
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "{\n"
              "auto node1 = doc.allocate_node(rapidxml::node_element, \"" << name_ << "\", doc.allocate_string(get_string(in." << name_ << ").c_str()));\n"
              "node->append_node(node1);\n"
              "}\n";
    }
};

// Concept requirements:
// sequence::value_type
// sequence::push_back
class xml_cpp_sequence: public detail::sequence
{
public:
    xml_cpp_sequence(const std::string & type, const std::string & name, const std::string & desc)
        : sequence(type, name, desc)
        , impl_type_("std::list< " + type + " >")
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, impl_type_);

        os << "// " << desc_ << "\n";
        os << impl_type_ << "    " << name_ << ";\n\n";
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, impl_type_);

        os << "if(!node || std::strcmp(node->name(), \"" << name_ << "\"))\n"
              "return false;\n"
              "{\n"
              "for(auto i = node->first_node(); i; i = i->next_sibling())\n"
              "{\n"
              "if(std::strcmp(i->name(), \"item\"))\n"
              "return false;"
           << impl_type_ << "::value_type tmp;\n"
              "if(!unserialize(i, tmp))\n"
              "return false;\n"
              "out." << name_ << ".push_back(tmp);\n"
              "}\n"
              "}\n"
              "node = node->next_sibling();\n";
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "{\n"
              "auto node1 = doc.allocate_node(rapidxml::node_element, doc.allocate_string(\"" << name_ << "\"));\n"
              "node->append_node(node1);\n"
              "for(auto i = in." << name_ << ".begin(); i != in." << name_ << ".end(); ++i)\n"
              "{\n"
              "auto node2 = doc.allocate_node(rapidxml::node_element, doc.allocate_string(\"item\"));\n"
              "node1->append_node(node2);\n"
           << xml_node_type_str << " tmp = 0;\n"
              "if(!serialize(tmp, *i, doc))\n"
              "return false;\n"
              "node2->append_node(tmp);\n"
              "}\n"
              "}\n";
    }
private:
    std::string impl_type_;
};

class xml_cpp_structure : public detail::structure
{
public:
    xml_cpp_structure(const std::string & type, const std::string & name, const std::string & desc)
        : structure(type, name, desc)
    {
    }

    void genDefinition(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "// " << desc_ << "\n";
        os << type_ << "    " << name_ << ";\n\n";
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "if(!node || std::strcmp(node->name(), \"" << name_ << "\"))\n"
              "return false;\n"
              "{\n"
              "auto tmp = node->first_node();\n"
              "if(!unserialize(tmp, out." << name_ << "))\n"
              "return false;\n"
              "}\n"
              "node = node->next_sibling();\n";
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, name_, type_);

        os << "{\n"
              "auto node1 = doc.allocate_node(rapidxml::node_element, \"" << name_ << "\");\n"
              "node->append_node(node1);\n"
           << xml_node_type_str << " node2 = 0;\n"
              "if(!serialize(node2, in." << name_ << ", doc))\n"
              "return false;\n"
              "node1->append_node(node2);\n"
              "}\n";
    }
};

class xml_cpp_entry : public detail::entry
{
public:
    xml_cpp_entry(const std::string & ns, const std::string & type, const std::string & desc, const std::string & id)
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
            os << "enum { id = " << id_ << " };\n\n";
        }

        for(auto i : items_)
        {
            i->genDefinition(os);
        }

        os << "};\n\n";
    }

    void genFuncDef(std::ostream & os) override
    {
        os << "bool serialize(" << xml_node_type_str << " , const " << namespace_ << "::" << type_ << " & ," << xml_document_type_str << " & );\n"
           << "bool unserialize(const " << xml_node_type_str << " , " << namespace_ << "::" << type_ << " & );\n\n";
    }

    void genUnserialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << "bool unserialize(const " << xml_node_type_str << " node, " << type_ << " & out)\n{\n"
              "node = node->first_node();\n\n";

        for(auto i : items_)
        {
            i->genUnserialize(os);
        }

        os << "return true;\n"
              "} \n"
              "\n";
    }

    void genSerialize(std::ostream & os) override
    {
        SERIALIZATION_TRACE(os, type_, objectType());

        os << "bool serialize(" << xml_node_type_str << " node, const " << type_ << " & in, " << xml_document_type_str << " & doc)\n"
              "{\n"
        for(auto i : items_)
        {
            i->genSerialize(os);
        }
        os << "return true;\n"
              "}\n"
              "\n";
    }
};

class xml_cpp_root : public detail::root
{
public:
    xml_cpp_root(const std::string & type, const std::string & desc)
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

        os << "namespace \n"
              "{\n"
              "\n"
              "template<class T>"
              "bool unserialize(const " << xml_node_type_str << " node, T & out)\n"
              "{\n"
              "    std::stringstream ss;\n"
              "    ss << node->value();\n"
              "    ss >> out;\n"
              "    return ss.eof();\n"
              "}\n"
              "\n"
              "template<>"
              "bool unserialize(const " << xml_node_type_str << " node, std::string & out)\n"
              "{\n"
              "    out = std::string(node->value()); \n"
              "    return true;\n"
              "}\n"
              "\n"
              "//for vector\n"
              "template<class T>\n"
              "bool serialize(" << xml_node_type_str << " & node, const T & in, " << xml_document_type_str << " & doc)\n"
              "{\n"
              "    std::stringstream ss;\n"
              "    ss << in;\n"
              "    node = doc.allocate_node(rapidxml::node_element, \"element\", doc.allocate_string(ss.str().c_str()));\n"
              "    return true;\n"
              "}\n"
              "\n"
              "// for base item\n"
              "// char or uchar is treated as interger\n"
              "template<class T>\n"
              "std::string get_string(const T & in)\n"
              "{\n"
              "    std::stringstream ss;\n"
              "    ss << in;\n"
              "    return ss.str();\n"
              "}\n"
              "\n"
              "} // namespace \n\n";

        os << "using namespace " << type_ << ";\n\n";
    }
};

struct xml_cpp_object_factory
{
public:
    typedef detail::item * item_type;
    typedef xml_cpp_entry * entry_type;
    typedef xml_cpp_root * root_type;

    item_type make_base(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new xml_cpp_base(type, name, desc);
    }

    item_type make_sequence(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new xml_cpp_sequence(type, name, desc);
    }

    item_type make_structure(const std::string & type, const std::string & name, const std::string & desc)
    {
        return new xml_cpp_structure(type, name, desc);
    }

    entry_type make_entry(const std::string & ns, const std::string & type, const std::string & desc, const std::string & id)
    {
        return new xml_cpp_entry(ns, type, desc, id);
    }

    root_type make_root(const std::string & type, const std::string & desc)
    {
        return new xml_cpp_root(type, desc);
    }

    root_type root;
};

template<template<typename ObjectFactory> class Parser>
class xml_cpp_generator
{
    typedef Parser<xml_cpp_object_factory> parser;
    typedef xml_cpp_object_factory factory;
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

        xml_cpp_object_factory factory;
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

        head << "#pragma once\n";
        head << "\n";
        head << "#include <list>\n";
        head << "#include <string>\n";
        head << "\n";
        head << "#include <rapidxml.hpp>\n";
        head << "\n\n";
        factory.root->genDefinition(head);

        cpp << "\n";
        cpp << "#include \"" << file << ".h" << "\"\n";
        cpp << "#include <cstring>\n";
        cpp << "#include <sstream>\n";
        cpp << "\n";
        cpp << "\n";
        factory.root->genBaseFunc(cpp);
        factory.root->genSerialize(cpp);
        factory.root->genUnserialize(cpp);
        return true;
    }
};
} // namespace serialization

