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
    int depth;
    std::vector <element*> children;
    std::vector <Parameter*> parameters;
};

void e_init(element* e);
element* parse(std::string str );
void print_tree(element* root);
void print_node(element* node);

element* find(std::string, element* node);
element* find_root(element* node);
#endif