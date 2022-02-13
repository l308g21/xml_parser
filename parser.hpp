#ifndef _H_PARSER_
#define _H_PARSER_

#include <string>
#include <vector>

struct element{
    std::string type;
    uint8_t depth;
    element* parent;
    std::vector <element> children;
    unsigned int child_n;
    std::string body;
    std::string values;
    
};

void e_init(element* e);
element* parse(std::string str );
#endif