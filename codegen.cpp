#include <iostream>
#include <cstring>

#include "generator.h"

struct CodeTypeInfo
{
    const char *name;
    serialization::generator::CodeType type;
};

int main(int argc, char *argv[])
{
    serialization::generator g;

    if(argc < 3)
    {
        std::cout << "invalid argument" << std::endl;
        return 0;
    }

    CodeTypeInfo types[] =
    {
        {"-binary", serialization::generator::BinaryCodeType, },
        {"-json", serialization::generator::JsonCodeType, },
        {"-xml", serialization::generator::XmlCodeType, },
    };

    for(int i = 0; i != sizeof(types)/sizeof(CodeTypeInfo); ++i)
    {
        if(strcmp(argv[1], types[i].name) == 0)
        {
            for(int j = 2; j < argc; ++j)
            {
                if(g.gen_code(types[i].type, argv[j]))
                    std::cout << argv[j] << ":  ok..." << std::endl;
            }
            return 0;
        }
    }

    std::cout << "invalid argument: " << argv[1] << std::endl;
    return 0;
}
