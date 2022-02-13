#include "parser.hpp"
#include <fstream>
#include <iostream>

void e_init(element* e){

    e->body = "";
    e->values = "";
    e->type = "undefined";
    e->parent = nullptr;
    e->child_n = 0;
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

int inspect_line(const char* line){
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



std::string get_type(std::string line){
    int index = 1;
    std::string type;

    while (line[index] != ' '){
        type += line[index];
        index++;
    }
    return type;
}



element* enter_new_element(element* current_element){
    if (current_element->parent!= nullptr){
        element* new_element = new element;
        e_init(new_element);
        new_element->parent = current_element;
        current_element = new_element;

    }    
    return current_element;
}



element* parse(std::string filename){

    element* root = new element;
    element* current_element;
    e_init(root);

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
    while (line_count < 9){
        if (file.eof()) break;
        file.getline(current_line, 150, '\n');
        std::cout << current_line << '\n';
        
        int result = inspect_line(current_line);
        switch( result){
            case line_element::OPEN:
                std::cout << "open new element" << std::endl;
                if (root-> type == "undefined"){

                }
                break;
            case line_element::META:
                std::cout << "just some meta info" << std::endl;
                break;
            case line_element::ONELINE:
                std::cout << "oneline no body" << std::endl;    
                break;
            case line_element::ONELINE_BODY:
                std::cout << "oneline with body" << std::endl;
                break;
            case line_element::CLOSE:
                std::cout << "close element" << std::endl;
                break;
            default:  break;  //there has been a formatting error
        }
       
        std::cout << '\n';

        line_count++;
    }
    file.close();

    return root;
}