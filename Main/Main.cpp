#include "ColPackHeaders.h"

using namespace ColPack;
using namespace std;

//Order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM
//Color: EXPLICIT_COVERING__STAR_BICOLORING, EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING, IMPLICIT_COVERING__STAR_BICOLORING, IMPLICIT_COVERING__GREEDY_STAR_BICOLORING
int main(int argc, const char* argv[]) {
    if(argc != 4) {
        cerr<<"usage: "<<argv[0]<<" GraphName WayToOrder DISTANCE_ONE/TWO"<<endl;
        cerr<<"Order: NATURAL, LARGEST_FIRST, DYNAMIC_LARGEST_FIRST, SMALLEST_LAST, INCIDENCE_DEGREE, RANDOM"<<endl;
        cerr<<"Distance: DISTANTCE_ONE, DISTANCE_TWO"<<endl;
        cerr<<"Color: EXPLICIT_COVERING__STAR_BICOLORING, EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING, IMPLICIT_COVERING__STAR_BICOLORING, IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"<<endl;
        return 0;
    }
	string s_InputFile(argv[1]);
	string s_order(argv[2]);
	string s_distance(argv[3]);

    GraphColoringInterface * g = new GraphColoringInterface(SRC_FILE, s_InputFile.c_str(), "AUTO_DETECTED");
	g->Coloring(s_order.c_str(), s_distance.c_str());
	cout << g->GetVertexColorCount()<<endl;

	delete g;
	return 0;
}


