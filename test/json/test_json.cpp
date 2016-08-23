
#include "test.h"
#include <rapidjson/writer.h>
#include <iostream>

int main()
{
	using namespace test;
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
	in.time = 15;
	in.arr[0] = 10;
	in.arr[1] = 9;
	in.mapxxx[3] = "string3";
	in.mapxxx[1] = "string1";
	in.mapxxx[2] = "string2";
	in.mapxxx[7] = "string7";
	in.mapxxx[8] = "string8";
	in.mapxxx[9] = "string9";
	in.mapxxx[0] = "string0";



	rapidjson::Value v(rapidjson::kObjectType);
	if(!serialize(v, in, doc))
	{
		std::cout << "serialize failed\n";
		return -1;
	}
	doc.AddMember("message",v,doc.GetAllocator());
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	doc.Accept(writer);
	std::string str(buffer.GetString(), buffer.GetSize());
	std::cout << str << std::endl;
	RetDeviceInfo info;
	rapidjson::Document doc2;
	doc2.Parse(str.c_str());
	if(doc.HasParseError())
	  return -1;
	if(!unserialize(doc2["message"], info))
	{
		std::cout << "unserialize failed\n";
		return -1;
	}
	return 0;

}
