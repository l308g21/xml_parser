#include "parser.hpp"
#include <iostream>

int main(void){
    
    Node* root = parse("testfiles/2021-05-09-16-17-39.gpx");
    Node* result = find("trk", root);
    Node* trackpt = find("trkpt", result);
    // print_tree(result);
    // print_node(trackpt);

    std::vector<Node*> track_segments = find_all("trkseg", result);
    std::cout << "there are " << track_segments.size() << " Nodes of type -trkseg-\n";
    // for (int i = 0; i < track_segments.size(); i++){
    //     print_node(track_segments[i]);
    // }

    bool is_match = match(trackpt, track_segments[0]->children[0]);
    if (is_match) std::cout << "it's a match!\n";
    else std::cout << "no match this time\n\n";
    is_match = match(trackpt, track_segments[0]->children[1]);
    if (is_match) std::cout << "it's a match!\n";
    else std::cout << "no match this time\n";
    return 0;
}