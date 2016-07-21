#include <fstream>
#include <iostream>
#include <sstream>

#include "codegen.h"
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

struct ReqDeviceInfo
{
    enum { id = 1 };
    int    userId;
    int    deviceId;
};

struct RetDeviceInfo
{
    enum { id = 2 };

    int    userId;
    int    deviceId;

    std::list< ReqDeviceInfo >    deviceInfos;

    std::list< int >    deviceInfos2;

    ReqDeviceInfo    deviceInfo;

    std::string time;
};

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

bool serialize(rapidjson::Value & val, const ReqDeviceInfo & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kObjectType);
    {
        rapidjson::Value v1;
        if(!serialize(v1, in.userId, doc))
            return false;
        v.AddMember("userId", std::move(v1), doc.GetAllocator());
    }
    {
        rapidjson::Value v1(rapidjson::kStringType);
        if(!serialize(v1, in.deviceId, doc))
                return false;
        v.AddMember("deviceId", std::move(v1), doc.GetAllocator());
    }
    val = std::move(v);
    return true;
}

bool serialize(rapidjson::Value & val, const RetDeviceInfo & in, rapidjson::Document & doc)
{
    rapidjson::Value v(rapidjson::kObjectType);
    {
        rapidjson::Value v1;
        if(!serialize(v1, in.userId, doc))
            return false;
        v.AddMember("userId", std::move(v1), doc.GetAllocator());
    }
    {
        rapidjson::Value v1;
        if(!serialize(v1, in.deviceId, doc))
            return false;
        v.AddMember("deviceId", std::move(v1), doc.GetAllocator());
    }
    {
        rapidjson::Value v1;
        if(!serialize(v1, in.deviceInfos, doc))
            return false;
        v.AddMember("deviceInfos", std::move(v1), doc.GetAllocator());
    }

    {
        rapidjson::Value v1;
        if(!serialize(v1, in.deviceInfos2, doc))
            return false;
        v.AddMember("deviceInfos2", std::move(v1), doc.GetAllocator());
    }

    {
        rapidjson::Value v1;
        if(!serialize(v1, in.deviceInfo, doc))
            return false;
        v.AddMember("deviceInfo", std::move(v1), doc.GetAllocator());
    }

    {
        rapidjson::Value v1;
        if(!serialize(v1, in.time, doc))
            return false;
        v.AddMember("time", std::move(v1), doc.GetAllocator());
    }
    val = std::move(v);
    return true;
}

//unserialize

bool unserialize(const rapidjson::Value & val, ReqDeviceInfo & out)
{
    if(val.IsNull())
        return false;

    if(!unserialize(val["userId"], out.userId))
        return false;
    if(!unserialize(val["deviceId"], out.deviceId))
        return false;

    return true;
}

bool unserialize(const rapidjson::Value & val, RetDeviceInfo & out)
{
    if(val.IsNull())
        return false;

    if(!unserialize(val["userId"], out.userId))
        return false;
    if(!unserialize(val["deviceId"], out.deviceId))
        return false;

    if(!unserialize(val["deviceInfos"], out.deviceInfos))
        return false;

    if(!unserialize(val["deviceInfos2"], out.deviceInfos2))
        return false;

    if(!unserialize(val["time"], out.time))
        return false;

    return true;
}


bool test()
{

    // parse json from string.
    rapidjson::Document doc;
    doc.SetObject();
    RetDeviceInfo in;
    in.userId = 1000;
    in.deviceId = 2000;
    in.deviceInfos.push_back(ReqDeviceInfo{1,2});
    in.deviceInfos.push_back(ReqDeviceInfo{3,4});
    in.deviceInfos.push_back(ReqDeviceInfo{5,6});
    in.deviceInfos.push_back(ReqDeviceInfo{7,8});
    in.deviceInfos2.push_back(1);
    in.deviceInfos2.push_back(2);
    in.deviceInfos2.push_back(3);
    in.deviceInfo.userId = 0;
    in.deviceInfo.deviceId = 2;
    in.time = "";

    rapidjson::Value v(rapidjson::kObjectType);
    serialize(v, in, doc);
    doc.AddMember("message",v,doc.GetAllocator());

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string json(buffer.GetString(), buffer.GetSize());
    std::cout << json << std::endl;

    //unserialize
    rapidjson::Document doc2;
    doc2.Parse(json.c_str());
    if(doc.HasParseError())
    {
        std::cout << "parse error: " << doc.GetParseError() << std::endl;
        return false;
    }
    RetDeviceInfo out;
    unserialize(doc2["message"], out);
    return true;
}

int main(int argc, char *argv[])
{
    test();
    generator g;

    for(int i = 1; i < argc; ++i)
    {
        try
        {
            g.gen(argv[i]);
            std::cout << argv[i] << ":  ok..." << std::endl;
        }
        catch(const serialization::parse_error& err)
        {
            std::cout << argv[i] << ">> " << err.where() << ":" << err.what() << std::endl;
        }
    }
}

