#include "test.h"

template<>
bool serialize(Buffer & buf, const test::ReqDeviceInfo & in)
{
if(!serialize(buf, in.userId))
return false;

if(!serialize(buf, in.deviceId))
return false;

return true;
}

template<>
bool serialize(Buffer & buf, const test::RetDeviceInfo & in)
{
if(!serialize(buf, in.userId))
return false;

if(!serialize(buf, in.deviceId))
return false;

if(!serialize(buf, in.name))
return false;

if(!serialize(buf, in.deviceInfos))
return false;

if(!serialize(buf, in.deviceInfos2))
return false;

if(!serialize(buf, in.deviceInfo))
return false;

if(!serialize(buf, in.time))
return false;

return true;
}

template<>
bool unserialize(const Buffer & buf, test::ReqDeviceInfo & out)
{
if(!unserialize(buf, out.userId))
return false;

if(!unserialize(buf, out.deviceId))
return false;


return true;
}

template<>
bool unserialize(const Buffer & buf, test::RetDeviceInfo & out)
{
if(!unserialize(buf, out.userId))
return false;

if(!unserialize(buf, out.deviceId))
return false;

if(!unserialize(buf, out.name))
return false;

if(!unserialize(buf, out.deviceInfos))
return false;

if(!unserialize(buf, out.deviceInfos2))
return false;

if(!unserialize(buf, out.deviceInfo))
return false;

if(!unserialize(buf, out.time))
return false;


return true;
}

