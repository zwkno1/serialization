 /*
 * Email:     zwkno1@gmail.com
 *
 */

#include "test.h"

#include <iostream>
#include <sstream>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

int main(int argc, char *argv[])
{
    std::string xml = "<message>"
                        "<userId>123</userId>"
                        "<deviceId>123243</deviceId>"
                        "<deviceInfos>"
							"<item>"
                                "<userId>7234</userId>"
                                "<deviceId>22334</deviceId>"
							"</item>"
							"<item>"
								"<userId>7234</userId>"
                                "<deviceId>22334</deviceId>"
							"</item>"
                        "</deviceInfos>"
                        "<deviceInfos2>"
                                 "<item>7234</userId>"
                                 "<item>7334</item>"
                                 "<item>42324</item>"
                        "</deviceInfos2>"
                        "<deviceInfo>"
                                "<userId>7234</userId>"
                                "<deviceId>22334</deviceId>"
                        "</deviceInfo>"
                        "<time>123243</time>"
						"<arr>"
							"<item>10</item>"
							"<item>9</item>"
							"<item>8</item>"
							"<item>7</item>"
							"<item>6</item>"
							"<item>5</item>"
							"<item>4</item>"
							"<item>3</item>"
							"<item>2</item>"
							"<item>1</item>"
						"</arr>"
                  "</message>";

    rapidxml::xml_document<> doc;
    doc.parse<0>((char *)xml.data());
    auto node = doc.first_node();
    test::RetDeviceInfo info;
    if(unserialize(node, info))
        std::cout << "xml to struct ok" << std::endl;
	else
        std::cout << "xml to struct failed" << std::endl;

    rapidxml::xml_document<> doc2;
	rapidxml::xml_node<> * node2 = doc2.allocate_node(rapidxml::node_element, "message");

    if(serialize(node2, info, doc2))
        std::cout << "struct to xml ok" << std::endl;
    doc2.append_node(node2);
    std::string strxml;
    rapidxml::print(std::back_inserter(strxml),doc2,0);
    std::cout << strxml << std::endl;
    std::cout << std::endl;
}
