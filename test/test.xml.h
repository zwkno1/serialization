#pragma once

#include <list>
#include <string>

#include <rapidxml.hpp>


namespace test
{
// qing qiu shebei xingxi
struct ReqDeviceInfo
{
enum { id = 1 };

// yong hu id
int    userId;

// she bei id
int    deviceId;

};

// fan hui shebei xingxi
struct RetDeviceInfo
{
enum { id = 2 };

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

bool serialize(rapidxml::xml_node<> * &, const test::ReqDeviceInfo & ,rapidxml::xml_document<> & );
bool unserialize(const rapidxml::xml_node<> * , test::ReqDeviceInfo & );

bool serialize(rapidxml::xml_node<> * &, const test::RetDeviceInfo & ,rapidxml::xml_document<> & );
bool unserialize(const rapidxml::xml_node<> * , test::RetDeviceInfo & );

