#include "parser.hpp"
#include <iostream>

int main(void){
    
    element* root = parse("testfiles/2021-05-09-16-17-39.gpx");
    element* result = find("trk", root);
    element* trackpt = find("trkpt", result);

    print_tree(result);
    print_node(trackpt);

    return 0;
}