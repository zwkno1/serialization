#include <iostream>
#include <cstring>

#include "generator.h"

int main(int argc, char *argv[])
{
    serialization::generator g;

    if(argc < 3)
    {
        std::cout << "invalid argument" << std::endl;
        return -1;
    }

    for(int j = 2; j < argc; ++j)
    {
        if(g.gen_code(argv[1], argv[j]))
            std::cout << argv[j] << ":  ok..." << std::endl;
	else
	    return -1;
    }
    
    return 0;
}
