#pragma once

#include "objects.h"

#include <map>

namespace serialization
{

namespace detail
{

class xml_parser
{
public:
    
    xml_parser(tree & t)
        : tree_(t)
    {
    }

    void parse(const std::string & xml)
    {
        std::string buf;
        buf = xml;
        parse(&buf[0]);
    }

    void parse(char * data)
    {
        rapidxml::xml_document<> doc;
        try
        {
            doc.parse<0>(data);
        }
        catch (const rapidxml::parse_error & err)
        {
            SERIALIZATION_PARSE_ERROR(err.what(), err.where<char>());
        }

        parse_root(doc.first_node());
    }
    
private:
    void parse_root(rapidxml::xml_node<> * node)
    {
        if(!node || std::strcmp(node->name(), "root"))
            SERIALIZATION_PARSE_ERROR("expect root", "");

        auto type = node->first_attribute("type");
        if(!type)
            SERIALIZATION_PARSE_ERROR("expect name", node->name());

        auto desc = node->first_attribute("desc");
        if(!desc)
            SERIALIZATION_PARSE_ERROR("expect desc", node->name());
        
        tree_.type = type->value();
        tree_.desc = desc->value();
              
        for(auto n = node->first_node(); n; n = n->next_sibling())
        {
            entry * e = parse_entry(n);
            tree_.entries.push_back(e);
        }
    }
    
    entry * parse_entry(rapidxml::xml_node<> *node)
    {
        if(std::strcmp(node->name(), "entry"))
            SERIALIZATION_PARSE_ERROR("expect entry", node->name());
        const char * type = parse_attribute(node, "type");
        const char * desc = parse_attribute(node, "desc");
        const char * id = parse_attribute(node, "id");

        if(*type == '\0')
            SERIALIZATION_PARSE_ERROR("expect entry name", node->name());

        entry * e = tree_.make_entry(type, desc, id);

        for(auto n = node->first_node(); n; n = n->next_sibling())
        {
            item * i = parse_item(n);
            e->items.push_back(i);
        }

        return e;
    }

    item * parse_item(rapidxml::xml_node<> *node)
    {
        const char * obj_type = node->name();
        const char * type = parse_attribute(node, "type");
        const char * name = parse_attribute(node, "name");
        const char * desc = parse_attribute(node, "desc");

        ItemType item_type = get_item_type_from_string(obj_type);
        if(item_type == MaxTypes)
        {
            SERIALIZATION_PARSE_ERROR("unexpect item type: ", obj_type);
        }

        item * i = tree_.make_item(item_type, type, name, desc);

        if(item_type == Array)
            if(!parse_attribute(node, "size", i->size))
                SERIALIZATION_PARSE_ERROR("expect item attr(size)", type);
        return i;
    }

    const char * parse_attribute(rapidxml::xml_node<> * node, const char * name)
    {
        auto attr = node->first_attribute(name);
        if(attr)
            return attr->value();
        return "";
    }

    template<typename T>
    bool parse_attribute(rapidxml::xml_node<> * node, const char * name, T & val)
    {
        auto attr = node->first_attribute(name);
        if(!attr)
            return false;
        std::stringstream ss;
        ss << attr->value();
        ss >> val;
        if(!ss.eof())
            return false;
        return true;
    }

    ItemType get_item_type_from_string(const char * type)
    {
        static std::map<std::string, ItemType> type_map =
        {
            {"base",    Base},
            {"array",   Array},
            {"list",    List},
            {"vector",  Vector},
            {"struct",  Struct},
        };

        auto iter = type_map.find(type);
        if(iter != type_map.end())
            return iter->second;
        return MaxTypes;
    }

private:
    tree & tree_;
};

} //namespace detail

} //namespace serialization
