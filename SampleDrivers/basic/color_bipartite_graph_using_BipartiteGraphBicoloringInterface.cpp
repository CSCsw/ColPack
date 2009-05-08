// An example for using BipartiteGraphBicoloringInterface to color Bipartite Graph
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
	BipartiteGraphBicoloringInterface *g = 
		new BipartiteGraphBicoloringInterface(
			s_InputFile, "AUTO_DETECTED"
			,"SMALLEST_LAST"
			,"IMPLICIT_COVERING__STAR_BICOLORING"
			);

	/*Done with coloring. Below are possible things that you may
	want to do after coloring:
	//*/

	//* 1. Check Star Bicoloring Coloring result
	cout<<"Check Star Bicoloring Coloring result ... "<<endl;
	g->CheckStarBicoloring();
	Pause();
	//*/

	//* 2. Print coloring results
	g->PrintVertexBicoloringMetrics();
	Pause();
	//*/

	//* 3. Get the list of colorID of colored vertices (in this case, the left side of the bipartite graph)
	vector<int> vi_LeftVertexColors;
	g->GetLeftVertexColors(vi_LeftVertexColors);

	vector<int> vi_RightVertexColors;
	g->GetRightVertexColors(vi_RightVertexColors);

	//Print Partial Colors
	g->PrintVertexBicolors();
	Pause();
	//*/

	//* 4. Get seed matrix
	int i_LeftSeedRowCount = 0;
	int i_LeftSeedColumnCount = 0;
	double** LeftSeed = g->GetLeftSeedMatrix(&i_LeftSeedRowCount, &i_LeftSeedColumnCount);

	int i_RightSeedRowCount = 0;
	int i_RightSeedColumnCount = 0;
	double** RightSeed = g->GetRightSeedMatrix(&i_RightSeedRowCount, &i_RightSeedColumnCount);

	//Display Seeds
	printf("Left Seed matrix %d x %d \n", i_LeftSeedRowCount, i_LeftSeedColumnCount);
	displayMatrix(LeftSeed, i_LeftSeedRowCount, i_LeftSeedColumnCount, 1);
	Pause();

	printf("Right Seed matrix %d x %d \n", i_RightSeedRowCount, i_RightSeedColumnCount);
	displayMatrix(RightSeed, i_RightSeedRowCount, i_RightSeedColumnCount, 1);
	Pause();
	//*/

	delete g;
	return 0;
}
//*/
