
#include "objects.h"

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

        ItemType item_type;
        if(!std::strcmp(obj_type, "base"))
        {
            item_type = Base;
        }
        else if(!std::strcmp(obj_type, "sequence"))
        {
            item_type = Sequence;
        }
        else if(!std::strcmp(obj_type, "structure"))
        {
            item_type = Structure;
        }
        else
        {
            SERIALIZATION_PARSE_ERROR("expect item(base or sequence or struct)", type);
        }
        return tree_.make_item(item_type, type, name, desc);
    }

    const char * parse_attribute(rapidxml::xml_node<> * node, const char * name)
    {
        auto attr = node->first_attribute(name);
        if(attr)
            return attr->value();
        return "";
    }
private:
    tree & tree_;
};

} //namespace detail

} //namespace serialization
