#include <iostream>
#include <cstring>

#include "generator.h"

int main(int argc, char *argv[])
{
    serialization::generator g;

    if(argc < 3)
    {
        std::cout << "invalid argument" << std::endl;
        return 0;
    }

    if(!std::strcmp(argv[1], "-xml"))
    {
        for(int i = 2; i < argc; ++i)
        {
            if(g.gen_xml_code(argv[i]))
                std::cout << argv[i] << ":  ok..." << std::endl;
        }
    }
    else if(!std::strcmp(argv[1], "-json"))
    {
        for(int i = 2; i < argc; ++i)
        {
            if(g.gen_json_code(argv[i]))
                std::cout << argv[i] << ":  ok..." << std::endl;
        }
    }
    else
    {
        std::cout << "invalid argument" << std::endl;
    }

    return 0;
}
