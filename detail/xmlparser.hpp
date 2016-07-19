
#include "objects.hpp"

namespace serialization
{

template<typename ObjectFactory>
class xml_parser
{
public:
    typedef detail::item * item_type;
    typedef detail::entry * entry_type;
    typedef typename ObjectFactory::root_type root_type;
    
    xml_parser(ObjectFactory & factory)
        : factory_(factory)
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

        auto root = doc.first_node();
        
        factory_.root = parse_root(root);
    }
    
private:
    root_type parse_root(rapidxml::xml_node<> * node)
    {
        if(!node || std::strcmp(node->name(), "root"))
            SERIALIZATION_PARSE_ERROR("expect root", "");

        auto type = node->first_attribute("type");
        if(!type)
            SERIALIZATION_PARSE_ERROR("expect name", node->name());

        auto desc = node->first_attribute("desc");
        if(!desc)
            SERIALIZATION_PARSE_ERROR("expect desc", node->name());
        
        root_type root = factory_.make_root(type->value(), desc->value());
              
        for(auto n = node->first_node(); n; n = n->next_sibling())
        {
            entry_type e = parse_entry(n, type->value());
            root->append(e);
        }
        return root;
    }
    
    entry_type parse_entry(rapidxml::xml_node<> *node, const std::string & ns)
    {
        if(std::strcmp(node->name(), "entry"))
            SERIALIZATION_PARSE_ERROR("expect entry", node->name());
        const char * type = parse_attribute(node, "type");
        const char * desc = parse_attribute(node, "desc");
        const char * id = parse_attribute(node, "id");

        if(*type == '\0')
            SERIALIZATION_PARSE_ERROR("expect entry name", node->name());

        entry_type entry = factory_.make_entry(ns, type, desc, id);

        for(auto n = node->first_node(); n; n = n->next_sibling())
        {
            item_type i = parse_item(n);
            entry->append(i);
        }

        return entry;
    }

    item_type parse_item(rapidxml::xml_node<> *node)
    {
        const char * obj_type = node->name();
        const char * type = parse_attribute(node, "type");
        const char * name = parse_attribute(node, "name");
        const char * desc = parse_attribute(node, "desc");

        if(!std::strcmp(obj_type, "base"))
        {
            return factory_.make_base(type, name, desc);
        }
        else if(!std::strcmp(obj_type, "sequence"))
        {
            return factory_.make_sequence(type, name, desc);
        }
        else if(!std::strcmp(obj_type, "structure"))
        {
            return factory_.make_structure(type, name, desc);
        }
        else
        {
            SERIALIZATION_PARSE_ERROR("expect item(base or sequence or struct)", type);
        }
    }

    const char * parse_attribute(rapidxml::xml_node<> * node, const char * name)
    {
        auto attr = node->first_attribute(name);
        if(attr)
            return attr->value();
        return "";
    }
private:
    ObjectFactory & factory_;
};

} //namespace serialization
