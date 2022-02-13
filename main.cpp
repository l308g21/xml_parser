#include "parser.hpp"
#include <iostream>

int main(void){
    
    std::cout << "what is going on here?\n";
    element* root = parse("testfiles/2021-05-09-16-17-39.gpx");

    return 0;
}