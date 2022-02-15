#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <string.h>

void e_init(element* e){
    e->type = "root";
    e->parent = nullptr;
    e->depth = 0;
    return;
}


int start_index(const char* str){
    int index = 0;
    while (str[index] != '\0'){   
        // trimming off the leading spaces
        if (str[index] ==  '<') return index ;
        index++;
    }
    // should never happen
    return -1;
}



enum line_element { META = 1, OPEN, ONELINE, ONELINE_BODY, CLOSE };

int categorize_line(const char* line){
    int index = start_index(line);
    int return_value = 0;
    switch (line[index + 1]){
        case '?':   return_value = line_element::META;   // open but its ??
                    break;
        case '/':   return_value = line_element::CLOSE;   // closes body
                    break;
        default:    return_value = line_element::OPEN;   //just opens
                    index += 2;
                    while (line[index] != '\0'){
                        if (line[index] == '/'){
                            if (line[index +1 ] == '>' ) {
                                return_value = line_element::ONELINE;   // closes in line and had no body
                                break;
                            }
                            else if (line[index -1 ] == '<' ) {
                                return_value = line_element::ONELINE_BODY;   // closes in line and had body
                                break;
                            }
                        }
                        index++;
                    }
                    // if return_value == line_element::FAIL => there seems to be a formatting error
    }
    return return_value;
}



std::string get_type(const char* line){
    int index = start_index(line) + 1; 
    std::string type;

    while (line[index] != ' ' && line[index] != '>'){
        type += line[index];
        index++;
    }
    return type;
}



element* enter_new_element(element* current_element){
    element* new_element = new element;
    e_init(new_element);
    new_element->parent = current_element;
    new_element->depth = new_element->parent->depth +1;
    new_element->parent->children.push_back(new_element);
    return new_element;
}



element* find_root( element* e){
    if (e->parent != nullptr) {
        // std::cout << "parent exists" << std::endl;
        return find_root(e->parent);
    }
    // std::cout << "there is no parent to this element" << std::endl;
    return e;
}



void resolve_parameters( element* current_element, const char* line){
    int index = start_index(line) + 1 + current_element->type.length();
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
        current_element->parameters.push_back(parameter);
        index++;
    }
    return;
}



element* parse(std::string filename){

    element* root = new element;
    element* current_element;
    e_init(root);
    current_element = root;

    char c;
    std::ifstream file( filename );
    if ( ! file ){
        std::cerr << "error: file could not be opened!\n";
        exit( 1 ); 
    }
    
    int depth = 0;
    int line_count = 0; 
    int index;
    char current_line[151];

    while (line_count < 111){
        if (file.eof()) break;
        file.getline(current_line, 150, '\n');
        // std::cout << current_line << '\n';
        
        int result = categorize_line(current_line);
        switch(result){
            case line_element::CLOSE: 
                current_element = current_element->parent;
                continue;
            case line_element::META:
                continue;
            default:
                current_element = enter_new_element(current_element);
                current_element->type = get_type(current_line);
                resolve_parameters(current_element, current_line);
                break;
        }

        if (result == line_element::ONELINE || result == line_element::ONELINE_BODY){
            current_element = current_element->parent;
        }
        // std::cout << '\n';
        line_count++;
    }
    
    root = find_root(current_element);
    print_tree( root );
    print_node( root );
    file.close();
    return root;
}



void print_node(element* node){
    std::cout << "type:  " << node->type << '\n';
    std::cout << "depth: " << node->depth << '\n';
    std::cout << "number of parameters: " << node->parameters.size() << '\n';
    std::cout << "number of children: " << node->children.size() << '\n';
    std::cout << "parent: " << node->parent->type << '\n';
}
void print_tree(element* root){
    int index = 0;
    while (index < root->depth){
        std::cout << '\t';
        index++;
    }
    std::cout << root->type << '\n';
    index = 0;
    while (index < root->children.size() ){
        print_tree(root->children[index]);
        index++;
    }
    return;
}