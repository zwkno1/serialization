#include <fstream>
#include <iostream>
#include <sstream>

#include "codegen.h"

int main(int argc, char *argv[])
{
    generator g;

    for(int i = 1; i < argc; ++i)
    {
        try
        {
            g.gen(argv[i]);
            std::cout << argv[i] << ":  ok..." << std::endl;
        }
        catch(const serialization::parse_error& err)
        {
            std::cout << argv[i] << ">> " << err.where() << ":" << err.what() << std::endl;

        }
    }
}

