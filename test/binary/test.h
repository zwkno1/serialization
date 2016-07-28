
#pragma once
#include <list>
#include <string>
#include <cstring>
#include <sstream>

#include "buffer.h"


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

// yong hu ming
std::string    name;

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
        template<typename T>
        bool serialize(Buffer & buf, const T & in)
        {
        buf.append(in);
        return true;
        }

        // for sequence
        template<typename T>
        bool serialize(Buffer & buf, const std::list<T> & in)
        {
        uint32_t size = in.size();
        buf.append(size);
        for(auto &i : in)
        {
        if(!serialize(buf, i))
        return false;
        }
        return true;
        }

        // for base item
        template<typename T>
        bool unserialize(const Buffer & buf, T & out)
        {
        return buf.get(out);
        }

        // for sequence
        template<typename T>
        bool unserialize(const Buffer & buf, std::list<T> & out)
        {
        uint32_t size;
        if(!buf.get(size))
        return false;

        T tmp;
        for(uint32_t i = 0; i != size; ++i)
        {
        if(!unserialize(buf, tmp))
        return false;
        }
        return true;
        }
        template<>
bool serialize(Buffer & , const test::ReqDeviceInfo & );
template<>
bool unserialize(const Buffer & , test::ReqDeviceInfo & );

template<>
bool serialize(Buffer & , const test::RetDeviceInfo & );
template<>
bool unserialize(const Buffer & , test::RetDeviceInfo & );

