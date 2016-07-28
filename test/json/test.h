
#pragma once
#include <list>
#include <string>
#include <sstream>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


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

template<>
bool serialize(rapidjson::Value & , const test::ReqDeviceInfo & , rapidjson::Document & );
template<>
bool unserialize(const rapidjson::Value & , test::ReqDeviceInfo & );

template<>
bool serialize(rapidjson::Value & , const test::RetDeviceInfo & , rapidjson::Document & );
template<>
bool unserialize(const rapidjson::Value & , test::RetDeviceInfo & );

