#include "xml_generator.h"

#include <fstream>
#include <sstream>

namespace serialization
{

namespace detail
{

void xml_generator::gen_code(const serialization::detail::tree & t, const std::string & name)
{
    std::ofstream h(name + ".h");
    std::ofstream cpp(name + ".cpp");

    //function def code
    std::stringstream os1;
    //serialize code
    std::stringstream os2;
    //unserialize code
    std::stringstream os3;

    for(serialization::detail::entry * e : t.entries)
    {
        os1 << "template<>\n"
               "bool serialize(rapidxml::xml_node<> * , const " << t.type << "::" << e->type << " & , rapidxml::xml_document<> & );\n"
            << "template<>\n"
               "bool unserialize(const rapidxml::xml_node<> *, " << t.type << "::" << e->type << " & );\n\n";

        os2 << "template<>\n"
               "bool serialize(rapidxml::xml_node<> * node, const " << t.type << "::" << e->type << " & in, rapidxml::xml_document<> & doc)\n"
               "{\n";

        os3 << "template<>\n"
               "bool unserialize(const rapidxml::xml_node<> * node, " << t.type << "::" << e->type << " & out)\n"
               "{\n"
               "if(!node)\n"
               "return false;"
               "node = node->first_node();\n\n";

        for(serialization::detail::item * i : e->items)
        {
            os2 << "{\n"
                   "auto node1 = doc.allocate_node(rapidxml::node_element, \"" << i->name << "\");\n"
                   "node->append_node(node1);\n"
                   "if(!serialize(node1, in." << i->name << ", doc))\n"
                   "return false;\n"
                   "}\n";

            os3 << "if(!node || std::strcmp(node->name(), \"" << i->name << "\"))\n"
                   "return false;\n"
                   "if(!unserialize(node, out." << i->name << "))\n"
                   "return false;\n"
                   "node = node->next_sibling();\n"
                   "\n";
        }

        os2 << "return true;\n"
               "}\n\n";

        os3 << "\nreturn true;\n"
               "}\n\n";
    }

    h << "\n"
         "#pragma once\n"
         "#include <list>\n"
         "#include <vector>\n"
         "#include <string>\n"
         "#include <cstring>\n"
         "#include <sstream>\n"
         "\n"
         "#include <rapidxml/rapidxml.hpp>\n"
         "\n"
         "\n";
    gen_struct_def(t, h);
    h << "#include \"xml_base.h\"\n\n";
    h << "\n\n";
    h << os1.rdbuf();

    cpp << "#include \"" << name << ".h\"\n"
           "\n";
    cpp << os2.rdbuf();
    cpp << os3.rdbuf();
}

} //namespace detail

} //namespace serialization
