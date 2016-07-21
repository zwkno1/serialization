
#include "test.h"
#include <cstring>
#include <sstream>


namespace 
{

template<class T>bool unserialize(const rapidxml::xml_node<> * node, T & out)
{
    std::stringstream ss;
    ss << node->value();
    ss >> out;
    return ss.eof();
}

template<>bool unserialize(const rapidxml::xml_node<> * node, std::string & out)
{
    out = std::string(node->value()); 
    return true;
}

// for base item
// char or uchar is treated as interger
template<class T>
bool serialize(rapidxml::xml_node<> * node, const T & in, rapidxml::xml_document<> & doc)
{
    std::stringstream ss;
    ss << in;
    node->value(doc.allocate_string(ss.str().c_str()));
    return true;
}

} // namespace 

using namespace test;

bool serialize(rapidxml::xml_node<> * node, const ReqDeviceInfo & in, rapidxml::xml_document<> & doc)
{
{
auto node1 = doc.allocate_node(rapidxml::node_element, "userId");
node->append_node(node1);
if(!serialize(node1, in.userId, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceId");
node->append_node(node1);
if(!serialize(node1, in.deviceId, doc))
return false;
}
return true;
}

bool serialize(rapidxml::xml_node<> * node, const RetDeviceInfo & in, rapidxml::xml_document<> & doc)
{
{
auto node1 = doc.allocate_node(rapidxml::node_element, "userId");
node->append_node(node1);
if(!serialize(node1, in.userId, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceId");
node->append_node(node1);
if(!serialize(node1, in.deviceId, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceInfos");
node->append_node(node1);
for(auto i = in.deviceInfos.begin(); i != in.deviceInfos.end(); ++i)
{
auto node2 = doc.allocate_node(rapidxml::node_element, "item");
node1->append_node(node2);
if(!serialize(node2, *i, doc))
return false;
}
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceInfos2");
node->append_node(node1);
for(auto i = in.deviceInfos2.begin(); i != in.deviceInfos2.end(); ++i)
{
auto node2 = doc.allocate_node(rapidxml::node_element, "item");
node1->append_node(node2);
if(!serialize(node2, *i, doc))
return false;
}
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceInfo");
node->append_node(node1);
rapidxml::xml_node<> * node2 = 0;
if(!serialize(node1, in.deviceInfo, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "time");
node->append_node(node1);
if(!serialize(node1, in.time, doc))
return false;
}
return true;
}

bool unserialize(const rapidxml::xml_node<> * node, ReqDeviceInfo & out)
{
node = node->first_node();

if(!node || std::strcmp(node->name(), "userId"))
return false;
if(!unserialize(node, out.userId))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "deviceId"))
return false;
if(!unserialize(node, out.deviceId))
return false;
node = node->next_sibling();

return true;
} 

bool unserialize(const rapidxml::xml_node<> * node, RetDeviceInfo & out)
{
node = node->first_node();

if(!node || std::strcmp(node->name(), "userId"))
return false;
if(!unserialize(node, out.userId))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "deviceId"))
return false;
if(!unserialize(node, out.deviceId))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "deviceInfos"))
return false;
{
for(auto i = node->first_node(); i; i = i->next_sibling())
{
if(std::strcmp(i->name(), "item"))
return false;
std::list< ReqDeviceInfo >::value_type tmp;
if(!unserialize(i, tmp))
return false;
out.deviceInfos.push_back(tmp);
}
}
node = node->next_sibling();
if(!node || std::strcmp(node->name(), "deviceInfos2"))
return false;
{
for(auto i = node->first_node(); i; i = i->next_sibling())
{
if(std::strcmp(i->name(), "item"))
return false;
std::list< int >::value_type tmp;
if(!unserialize(i, tmp))
return false;
out.deviceInfos2.push_back(tmp);
}
}
node = node->next_sibling();
if(!node || std::strcmp(node->name(), "deviceInfo"))
return false;
{
if(!unserialize(node, out.deviceInfo))
return false;
}
node = node->next_sibling();
if(!node || std::strcmp(node->name(), "time"))
return false;
if(!unserialize(node, out.time))
return false;
node = node->next_sibling();

return true;
} 

