 /*
 * Email:     zwkno1@gmail.com
 *
 */

#include "test.xml.h"

#include <iostream>
#include <sstream>
#include <rapidxml.hpp>
#include <rapidxml_print.hpp>

int main(int argc, char *argv[])
{
    std::string xml = "<RetDeviceInfo>"
                        "<userId>123</userId>"
                        "<deviceId>123243</deviceId>"
                        "<deviceInfos>"
                            "<Vector>"
                                "<ReqDeviceInfo>"
                                    "<userId>7234</userId>"
                                    "<deviceId>22334</deviceId>"
                                "</ReqDeviceInfo>"
                                "<ReqDeviceInfo>"
                                    "<userId>7234</userId>"
                                    "<deviceId>22334</deviceId>"
                                "</ReqDeviceInfo>"
                            "</Vector>"
                        "</deviceInfos>"
                        "<deviceInfos2>"
                            "<Vector>"
                                 "<userId>7234</userId>"
                                 "<userId>7334</userId>"
                                 "<userId>42324</userId>"
                            "</Vector>"
                        "</deviceInfos2>"
                        "<deviceInfo>"
                            "<ReqDeviceInfo>"
                                "<userId>7234</userId>"
                                "<deviceId>22334</deviceId>"
                            "</ReqDeviceInfo>"
                        "</deviceInfo>"
                        "<time>123243</time>"
                  "</ReqDeviceInfo>";

    rapidxml::xml_document<> doc;
    doc.parse<0>((char *)xml.data());
    auto node = doc.first_node();
    test::RetDeviceInfo info;
    if(unserialize(node, info))
        std::cout << "xml to struct ok" << std::endl;

    rapidxml::xml_document<> doc2;
    rapidxml::xml_node<> *node2 = 0;
    if(serialize(node2, info, doc2))
        std::cout << "struct to xml ok" << std::endl;
    doc2.append_node(node2);
    std::string strxml;
    rapidxml::print(std::back_inserter(strxml),doc2,0);
    std::cout << strxml << std::endl;
    std::cout << std::endl;
}
