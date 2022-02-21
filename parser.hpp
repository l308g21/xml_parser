#ifndef _H_PARSER_
#define _H_PARSER_

#include <string>
#include <vector>


struct Parameter{
    std::string name;
    std::string value;
};

struct Node{
    std::string type;
    Node*    parent;
    int         depth;
    std::vector <Node*>      children;
    std::vector <Parameter*>    parameters;
};

Node* parse  ( std::string str );
void delete_node( Node* node );
void delete_tree( Node* node );
void print_tree ( Node* root );
void print_node ( Node* node );

Node* find       ( std::string name, Node* node );
Node* find_root  ( Node* node );
std::vector<Node*> find_all( std::string, Node* node );

bool match( Node* node1, Node* node2 );
Parameter* find_parameter(std::string name, Node* node);

#endif // _H_PARSER_