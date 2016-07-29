
#pragma once
#include <list>
#include <string>
#include <cstring>
#include <sstream>

#include <rapidxml/rapidxml.hpp>


#pragma pack(push,1)

// for test
namespace test
{
// qing qiu shebei xingxi
struct ReqDeviceInfo
{
enum { ID = 1 };

// yong hu id
int    userId;

// she bei id
int    deviceId;

};

// fan hui shebei xingxi
struct RetDeviceInfo
{
enum { ID = 2 };

// yong hu id
int    userId;

// she bei id
int    deviceId;

// she bei zhuang tai
std::list< ReqDeviceInfo >    deviceInfos;

// she bei zhuang tai
std::list< int >    deviceInfos2;

// she bei zhuang tai
ReqDeviceInfo    deviceInfo;

// she bei shi jian
int    time;

};


} // namespace test


#pragma pack(pop)


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
template<>
bool serialize(rapidxml::xml_node<> * , const test::ReqDeviceInfo & , rapidxml::xml_document<> & );
template<>
bool unserialize(const rapidxml::xml_node<> *, test::ReqDeviceInfo & );

template<>
bool serialize(rapidxml::xml_node<> * , const test::RetDeviceInfo & , rapidxml::xml_document<> & );
template<>
bool unserialize(const rapidxml::xml_node<> *, test::RetDeviceInfo & );

