#include "parser.hpp"
#include <iostream>

int main(void){
    
    element* root = parse("testfiles/2021-05-09-16-17-39.gpx");
    element* result = find("trk", root);
    element* trackpt = find("trkpt", result);

    // print_tree(result);
    // print_node(trackpt);

    std::vector<element*> track_segments = find_all("trkseg", result);
    std::cout << "there are " << track_segments.size() << " elements of type -trkseg-\n";
    // for (int i = 0; i < track_segments.size(); i++){
    //     print_node(track_segments[i]);
    // }

    return 0;
}