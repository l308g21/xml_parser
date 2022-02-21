#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

void e_init( Node* e ){
    e->type = "root";
    e->parent = nullptr;
    e->depth = 0;
    return;
}


int start_index( const char* str ){
    int index = 0;
    while (str[index] != '\0'){   
        // trimming off the leading spaces
        if (str[index] ==  '<') return index ;
        index++;
    }
    // should never happen
    return -1;
}



enum line_Node { META = 1, OPEN, ONELINE, ONELINE_BODY, CLOSE };

int categorize_line( const char* line ){
    int index = start_index(line);
    int return_value = 0;
    switch (line[index + 1]){
        case '?':   return_value = line_Node::META;   // open but its ??
                    break;
        case '/':   return_value = line_Node::CLOSE;   // closes body
                    break;
        default:    return_value = line_Node::OPEN;   //just opens
                    index += 2;
                    while (line[index] != '\0'){
                        if (line[index] == '/'){
                            if (line[index +1 ] == '>' ) {
                                return_value = line_Node::ONELINE;   // closes in line and had no body
                                break;
                            }
                            else if (line[index -1 ] == '<' ) {
                                return_value = line_Node::ONELINE_BODY;   // closes in line and had body
                                break;
                            }
                        }
                        index++;
                    }
                    // if return_value == line_Node::FAIL => there seems to be a formatting error
    }
    return return_value;
}



std::string get_type( const char* line ){
    int index = start_index(line) + 1; 
    std::string type;
    while (line[index] != ' ' && line[index] != '>'){
        type += line[index];
        index++;
    }
    return type;
}



Node* enter_new_Node( Node* current_Node ){
    Node* new_Node = new Node;
    e_init( new_Node );
    new_Node->parent = current_Node;
    new_Node->depth  = new_Node->parent->depth + 1;
    new_Node->parent->children.push_back( new_Node );
    return new_Node;
}



void resolve_parameters( Node* current_Node, const char* line ){
    int index = start_index( line ) + 1 + current_Node->type.length();
    while(line[index] != '>' && line[index] != '/'){
        //if previously read parameter index points to space -- catch that
        if (line[index] == ' '){
            index++;
            continue;
        }
        Parameter* parameter = new Parameter;
        while (line[index] != '='){
            parameter->name += line[index];
            index++;
        }
        // index currently points at '=' next is '"' then follows the value
        index += 2;
        while (line [index] != '"'){
            parameter->value += line[index];
            index++;
        }
        // fully read one parameter now add it to current_parameter
        current_Node->parameters.push_back(parameter);
        index++;
    }
    return;
}



void process_oneline_body( Node* current_Node, const char* line ){
    // oneline with body is indefferent to parameter to parent object
    std::string name = get_type(line);
    int index = start_index(line) + 1 + name.length() + 1;
    Parameter* parameter = new Parameter;
    parameter->name = name;
    while(line[index] != '<'){
        parameter->value += line[index];
        index++;
    }
    current_Node->parameters.push_back(parameter);
    return;

}


Node* parse( std::string filename ){
    Node* root = new Node;
    Node* current_Node;
    e_init(root);
    current_Node = root;

    char c;
    std::ifstream file( filename );
    if ( ! file ){
        std::cerr << "error: file could not be opened!\n";
        exit( 1 ); 
    }
    
    int depth       = 0;
    int line_count  = 0; 
    char current_line[151];     // just some random length that happened to be longer than any lines in my testfile. maybe making use of std::string would be preferable

    while (line_count < 112){
        if (file.eof()) break;
        file.getline(current_line, 150, '\n');
        // std::cout << current_line << '\n';
        
        int result = categorize_line( current_line );
        switch(result){
            case line_Node::CLOSE: 
                    current_Node = current_Node->parent;
                    continue;
            case line_Node::META:
                    continue;
            case line_Node::ONELINE_BODY:
                    process_oneline_body( current_Node, current_line );
                    break;
            default:
                    current_Node         = enter_new_Node( current_Node );
                    current_Node->type   = get_type( current_line );
                    resolve_parameters( current_Node, current_line );
                    break;
        }

        if(result == line_Node::ONELINE) current_Node = current_Node->parent;
        line_count++;
    }
    
    file.close();
    return root;
}



void print_node( Node* node ){
    std::cout << "type:  " << node->type << '\n';
    std::cout << "depth: " << node->depth << '\n';
    std::cout << "number of parameters: " << node->parameters.size() << '\n';
    std::cout << "number of children: " << node->children.size() << '\n';
    if ( node->parent != nullptr )    std::cout << "parent: " << node->parent->type << '\n';
    else    std::cout << "node has no parent\n";
    return;
}



void print_tree( Node* root ){
    int index = 0;
    while ( index < root->depth ){
        std::cout << '\t';
        index++;
    }
    std::cout << root->type << '\n';
    index = 0;
    while (index < root->children.size() ){
        print_tree(root->children[index] );
        index++;
    }
    return;
}



// this would be a great opportunity for making use of namespaces
Node* find( std::string name, Node* node ){
    if ( node->type == name ) return node;
    int index = 0;
    Node* result;
    while ( index < node->children.size() ){
        result = find( name, node->children[index] );
        if ( result != nullptr ) return result;
        index++;
    }
    return nullptr;
}



Node* find_root( Node* e){
    if ( e->parent != nullptr ) {
        return find_root( e->parent );
    }
    return e;
}



std::vector<Node*> find_all( std::string name, Node* node ){
    std::vector<Node*> nodes;
    if ( node->type == name ) nodes.push_back( node );
    int index = 0;
    std::vector<Node*> result;
    while ( index < node->children.size() ){
        result = find_all( name,node->children[index] );
        for ( int i = 0; i < result.size(); i++ )     nodes.push_back( result[i] );
        result.clear();
        index++;
    }
    return nodes;               // counting on compiler optimization to perform efficient return
}



bool match( Node* node1, Node* node2 ){
    if ( node1->type != node1->type )   return false;
    if ( node1->depth != node2->depth ) return false;
    if ( node1->parameters.size() != node2->parameters.size() ) return false;
    if ( node1->children.size() != node2->children.size() )     return false;

    int index = 0;
    while ( index < node1->parameters.size() ){
        if ( node1->parameters[index]->name != node2->parameters[index]->name )   return false;
        if ( node1->parameters[index]->value != node2->parameters[index]->value ) return false;
        index++;
    }
    index = 0;
    bool is_match;
    while ( index < node1->children.size() ){
        if ( !match( node1->children[index], node2->children[index]) ) return false;
        index++;
    }
    return true;
}



void delete_node(Node* node){
    while ( !node->children.empty() ){
        delete_node(node->children[0]);
    }
    // no children left --> delete this node
    // remove from parent
    int index = 0;
    while ( index < node->parent->children.size() ){
        if ( match( node->parent->children[index], node ) ) break;
        index++;
    }
    node->parent->children.erase( node->parent->children.begin() + index );
    
    index = 0;
    while (index < node->parameters.size() ){
        delete node->parameters[index];
        index++;
    }

    delete node;
    return;
}



void delete_tree(Node* node){
    delete_node( find_root( node ) );
    return;
}



Parameter* find_parameter( std::string name, Node* node ){
    int index = 0;
    while ( index < node->parameters.size() ){
        if ( node->parameters[index]->name == name ) return node->parameters[index];
        index++;
    }
    return nullptr;
}