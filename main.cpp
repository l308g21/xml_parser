#include "parser.hpp"
#include <iostream>


int main(void){
    
    Node* root = parse("testfiles/2021-05-09-16-17-39.gpx");

    std::vector< Node* > points = find_all( "trkpt", root );
    if (! points.empty() ) std::cout << "found some points\n";

    Node* bounds = find("bounds", root);
    float minlat = atof( find_parameter( "minlat", bounds )->value.c_str() );
    float minlon = atof( find_parameter( "minlon", bounds )->value.c_str() );
    std::cout << "minlat: " << minlat << '\n';
    std::cout << "minlon: " << minlon << '\n';
    return 0;
}