#ifndef _H_PARSER_
#define _H_PARSER_

#include <string>
#include <vector>


struct Parameter{
    std::string name;
    std::string value;
};


struct element{
    std::string type;
    element* parent;
    std::vector <element*> children;
    std::vector <Parameter*> parameters;
};

void e_init(element* e);
element* parse(std::string str );
#endif