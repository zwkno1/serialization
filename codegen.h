
#include "config.h"

#ifdef XML_PARSER
#include "detail/xmlparser.hpp"
template<typename T>
using parser = serialization::xml_parser<T>;
#else
#error "no parser"
#endif

#ifdef XML_CPP_GENERATOR
#include "detail/xmlcppgenerator.hpp"
typedef serialization::xml_cpp_generator<parser> generator;
#else
#error "no generator"
#endif
    


