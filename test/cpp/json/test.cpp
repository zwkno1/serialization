#include "test.h"

#include <cstring>

template<>
bool serialize(rapidjson::Value & val, const test::ReqDeviceInfo & in, rapidjson::Document & doc)
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
val = std::move(v);
return true;
}

template<>
bool serialize(rapidjson::Value & val, const test::RetDeviceInfo & in, rapidjson::Document & doc)
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

template<>
bool unserialize(const rapidjson::Value & val, test::ReqDeviceInfo & out)
{
if(val.IsNull())
return false;

if(!unserialize(val["userId"], out.userId))
return false;
if(!unserialize(val["deviceId"], out.deviceId))
return false;

return true;
}

template<>
bool unserialize(const rapidjson::Value & val, test::RetDeviceInfo & out)
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
if(!unserialize(val["deviceInfo"], out.deviceInfo))
return false;
if(!unserialize(val["time"], out.time))
return false;

return true;
}

