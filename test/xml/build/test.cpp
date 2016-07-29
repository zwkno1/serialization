#include "test.h"

template<>
bool serialize(rapidxml::xml_node<> * node, const test::ReqDeviceInfo & in, rapidxml::xml_document<> & doc)
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

template<>
bool serialize(rapidxml::xml_node<> * node, const test::RetDeviceInfo & in, rapidxml::xml_document<> & doc)
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
if(!serialize(node1, in.deviceInfos, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceInfos2");
node->append_node(node1);
if(!serialize(node1, in.deviceInfos2, doc))
return false;
}
{
auto node1 = doc.allocate_node(rapidxml::node_element, "deviceInfo");
node->append_node(node1);
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

template<>
bool unserialize(const rapidxml::xml_node<> * node, test::ReqDeviceInfo & out)
{
if(!node)
return false;node = node->first_node();

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

template<>
bool unserialize(const rapidxml::xml_node<> * node, test::RetDeviceInfo & out)
{
if(!node)
return false;node = node->first_node();

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
if(!unserialize(node, out.deviceInfos))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "deviceInfos2"))
return false;
if(!unserialize(node, out.deviceInfos2))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "deviceInfo"))
return false;
if(!unserialize(node, out.deviceInfo))
return false;
node = node->next_sibling();

if(!node || std::strcmp(node->name(), "time"))
return false;
if(!unserialize(node, out.time))
return false;
node = node->next_sibling();


return true;
}

