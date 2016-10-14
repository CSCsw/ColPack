#include "ColPackHeaders.h"

using namespace ColPack;
using namespace std;

//Order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM
//Color: EXPLICIT_COVERING__STAR_BICOLORING, EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING, IMPLICIT_COVERING__STAR_BICOLORING, IMPLICIT_COVERING__GREEDY_STAR_BICOLORING
int main(int argc, const char* argv[]) {
    if(argc != 2) {
        cerr<<"usage: "<<argv[0]<<" GraphName"<<endl;
        cerr<<"Order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM"<<endl;
        cerr<<"Distance: DISTANTCE_ONE, DISTANCE_TWO"<<endl;
        cerr<<"Color: EXPLICIT_COVERING__STAR_BICOLORING, EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING, IMPLICIT_COVERING__STAR_BICOLORING, IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"<<endl;
        return 0;
    }

    GraphColoringInterface * g = new GraphColoringInterface(SRC_FILE, argv[1], "AUTO_DETECTED");
    cout<< "DISTANCE_ONE COLOR Result:" << endl;
    g->Coloring("NATURAL", "DISTANCE_ONE");
    cout<<"  NATURAL              : " << g->GetVertexColorCount()<<endl;
    g->Coloring("LARGEST_FIRST", "DISTANCE_ONE");
    cout<<"  LARGEST_FIRST        : " << g->GetVertexColorCount()<<endl;
    g->Coloring("DYNAMIC_LARGEST_FIRST", "DISTANCE_ONE");
    cout<<"  DYNAMIC_LARGEST_FIRST: " << g->GetVertexColorCount()<<endl;
    g->Coloring("SMALLEST_LAST", "DISTANCE_ONE");
    cout<<"  SMALLEST_LAST        : " << g->GetVertexColorCount()<<endl;
    g->Coloring("INCIDENCE_DEGREE", "DISTANCE_ONE");
    cout<<"  INCIDENCE_DEGREE     : " << g->GetVertexColorCount()<<endl;
    g->Coloring("RANDOM", "DISTANCE_ONE");
    cout<<"  RANDOM               : " << g->GetVertexColorCount()<<endl;

    cout<<endl;
    cout<< "DISTANCE_TWO COLOR Result:" << endl;
    g->Coloring("NATURAL", "DISTANCE_TWO");
    cout<<"  NATURAL              : " << g->GetVertexColorCount()<<endl;
    g->Coloring("LARGEST_FIRST", "DISTANCE_ONE");
    cout<<"  LARGEST_FIRST        : " << g->GetVertexColorCount()<<endl;
    g->Coloring("DYNAMIC_LARGEST_FIRST", "DISTANCE_ONE");
    cout<<"  DYNAMIC_LARGEST_FIRST: " << g->GetVertexColorCount()<<endl;
    g->Coloring("SMALLEST_LAST", "DISTANCE_ONE");
    cout<<"  SMALLEST_LAST        : " << g->GetVertexColorCount()<<endl;
    g->Coloring("INCIDENCE_DEGREE", "DISTANCE_ONE");
    cout<<"  INCIDENCE_DEGREE     : " << g->GetVertexColorCount()<<endl;
    g->Coloring("RANDOM", "DISTANCE_ONE");
    cout<<"  RANDOM               : " << g->GetVertexColorCount()<<endl;

    delete g;
    return 0;
}


