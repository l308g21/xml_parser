#ifndef _H_PARSER_
#define _H_PARSER_

#include <string>
#include <vector>


struct parameter{
    std::string name;
    std::string value;
};


struct element{
    std::string type;
    element* parent;
    std::vector <element*> children;
    std::vector <parameter> parameters;
};

void e_init(element* e);
element* parse(std::string str );
#endif