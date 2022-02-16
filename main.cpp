#include "parser.hpp"
#include <iostream>

int main(void){
    
    element* root = parse("testfiles/2021-05-09-16-17-39.gpx");
    element* result = find("trk", root);

    print_tree(result);
    print_node(result);
    
    return 0;
}