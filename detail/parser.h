#pragma once

#include "config.h"

#ifdef XML_PARSER
#include "detail/xml_parser.h"
typedef serialization::detail::xml_parser parser;
#else
#error "no parser"
#endif
