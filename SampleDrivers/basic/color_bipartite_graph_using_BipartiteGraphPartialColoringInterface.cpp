// An example for using BipartiteGraphPartialColoringInterface to color Bipartite Graph
/*
How to use this driver:
	To compile the code, replace the Main.cpp file in Main directory with this file
	and run "make" in ColPack installation directory

Any time you have trouble understanding what a routine does, how to use a routine, or what are the accepted values for a parameter, 
please reference the COLPACK's online documentation (temporarily located at 
http://www.cs.odu.edu/~dnguyen/dox/colpack/html/ ).

For more information, please visit our webpage http://www.cs.odu.edu/~assefaw/coloringpage/
//*/

#include "ColPackHeaders.h"

using namespace COLPACK;
using namespace std;

// baseDir should point to the directory (folder) containing the input file
string baseDir="C:\\Duck\\research\\graph\\MM - Eric Phipps\\";

//*	A SHORT VERSION
int main(int argc, char ** argv)
{
	// s_InputFile = baseDir + <name of the input file>
	string s_InputFile; //path of the input file
	s_InputFile = baseDir + "element_sparsity\\element_sparsity.mtx"; 

	//Generate and color the bipartite graph
	BipartiteGraphPartialColoringInterface *g = 
		new BipartiteGraphPartialColoringInterface(
			s_InputFile,	"AUTO_DETECTED"
			,"LARGEST_FIRST"
			,"LEFT_PARTIAL_DISTANCE_TWO"
			);

	/*Done with coloring. Below are possible things that you may
	want to do after coloring:
	//*/

	/* 1. Check Partial Distance Two Coloring result
	cout<<"Check Partial Distance Two coloring result ... "<<endl;
	if(g->CheckPartialDistanceTwoColoring() == _FALSE) cout<<" FAILED"<<endl;
	else cout<<" SUCCEEDED"<<endl;
	Pause();
	//*/

	//* 2. Print coloring results
	g->PrintPartialColoringMetrics();
	Pause();
	//*/

	//* 3. Get the list of colorID of colored vertices (in this case, the left side of the bipartite graph)
	vector<int> vi_VertexPartialColors;
	g->GetVertexPartialColors(vi_VertexPartialColors);

	//Print Partial Colors
	g->PrintPartialColors();
	Pause();
	//*/

	/* 4. Get seed matrix
	int i_SeedRowCount = 0;
	int i_SeedColumnCount = 0;
	double** Seed = g->GetSeedMatrix(&i_SeedRowCount, &i_SeedColumnCount);

	//Display Seed
	printf("Seed matrix %d x %d \n", i_SeedRowCount, i_SeedColumnCount);
	displayMatrix(Seed, i_SeedRowCount, i_SeedColumnCount, 1);
	Pause();
	//*/

	delete g;
	return 0;
}
//*/
