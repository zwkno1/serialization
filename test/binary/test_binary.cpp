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
	info.arr[0] = 10;
	info.arr[1] = 9;
	info.arr[2] = 8;
	info.arr[3] = 7;
	info.arr[4] = 6;
	info.arr[5] = 5;
	info.arr[6] = 4;
	info.arr[7] = 3;
	info.arr[8] = 2;
	info.arr[9] = 1;
	info.name_list[0] = "01234567890";
	info.name_list[1] = "`1234567890-=qwertyuiop[]asdfghjkl;'zxcvbnm,./";
	info.name_list[2] = "123321";
	info.name_list[3] = "sjiduhfkaishdfklhjasdfkljhasdfjaskjdhfiqy23rjkejhfoudsfjgkqhwlreiidksgfbliuh";
	info.mapxxx[3] = "string3";
	info.mapxxx[1] = "string1";
	info.mapxxx[2] = "string2";
	info.mapxxx[7] = "string7";
	info.mapxxx[8] = "string8";
	info.mapxxx[9] = "string9";
	info.mapxxx[0] = "string0";

		
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
