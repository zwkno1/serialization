#include <iostream>
#include <fstream>
#include "test.h"
#include "buffer.h"

int main(int argc, char *argv[])
{
	test::RetDeviceInfo info;
	info.deviceId = 1000;
	info.userId = 20000;
	info.name = "yigedashabi";
	info.deviceInfos.push_back(test::ReqDeviceInfo{1000, 11111});
	info.deviceInfos.push_back(test::ReqDeviceInfo{2000, 22222});
	info.deviceInfos.push_back(test::ReqDeviceInfo{3000, 33333});
	info.deviceInfos.push_back(test::ReqDeviceInfo{4000, 44444});
	info.deviceInfos2.push_back(100);
	info.deviceInfos2.push_back(200);
	info.deviceInfos2.push_back(300);
	info.deviceInfos2.push_back(400);
	info.deviceInfos2.push_back(500);
	info.deviceInfos2.push_back(600);
	info.deviceInfo.userId = 8000;
	info.deviceInfo.userId = 9000;
	info.time = 1098090;
		
	Buffer buf;
	if(!serialize(buf, info))
	  return -1;

	std::ofstream f("test.out");
	f.write((const char *)buf.data(), buf.size());

	std::cout << "serialize ok" << std::endl;

	test::RetDeviceInfo info2;
    if(!unserialize(buf, info2))
	  return -1;
	std::cout << "unserialize ok" << std::endl;

	return 0;

}
