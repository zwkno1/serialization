#include <fstream>
#include <iostream>
#include <sstream>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "generator.h"

int main(int argc, char *argv[])
{
    rapidjson::Value v;
    serialization::generator g;

    for(int i = 1; i < argc; ++i)
    {
        if(g.gen_json_code(argv[i]))
            std::cout << argv[i] << ":  ok..." << std::endl;
    }
}

