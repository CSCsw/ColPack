// An example for using GraphColoringInterface to color Graph
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
#ifdef ____WINDOWS_OS____
	string baseDir="C:\\Duck\\research\\graph\\MM collection\\";
	//string baseDir="C:\\Duck\\research\\graph\\Bozdagd\\application\\";
	//string baseDir="C:\\Duck\\research\\COLPACK-1.0.0\\Graphs\\";
	//string baseDir="C:\\Duck\\research\\graph\\Eric Phillip\\";
#else // UNIX
	string baseDir="/home/dnguyen/duck/Research/graph/MM_Graph/";
	//string baseDir="/home/dnguyen/duck/Research/graph/bozdagd/synthetic/";
	//string baseDir="/home/dnguyen/duck/Research/graph/bozdagd/application/";
#endif

//*	A SHORT VERSION
int main(int argc, char ** argv)
{
	// s_InputFile = baseDir + <name of the input file>
	string s_InputFile; //path of the input file. PICK A SYMMETRIC MATRIX!!!
	s_InputFile = baseDir + "bcsstk01_symmetric\\bcsstk01_symmetric.mtx"; 

	//Generate and color the graph
	GraphColoringInterface * g = new GraphColoringInterface(
		s_InputFile, "AUTO_DETECTED",
		"LARGEST_FIRST",
		"DISTANCE_TWO"
		);

	/*Done with coloring. Below are possible things that you may
	want to do after coloring:
	//*/

	/* 1. Check DISTANCE_TWO coloring result
	cout<<"Check DISTANCE_TWO coloring result"<<endl;
	g->CheckDistanceTwoColoring();
	Pause();
	//*/

	//* 2. Print coloring results
	g->PrintVertexColoringMetrics();
	Pause();
	//*/

	//* 3. Get the list of colorID of vertices
	vector<int> vi_VertexColors;
	g->GetVertexColors(vi_VertexColors);

	//Display vector of VertexColors
	printf("vector of VertexColors (size %d) \n", vi_VertexColors.size());
	displayVector(&vi_VertexColors[0], vi_VertexColors.size(), 1);
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

/* A LONGER VERSION showing steps actually executed by the constructor.
int main(int argc, char ** argv)
{
	// s_InputFile = baseDir + <name of the input file>
	string s_InputFile; //path of the input file. PICK A SYMMETRIC MATRIX!!!
	s_InputFile = baseDir + "bcsstk01_symmetric\\bcsstk01_symmetric.mtx"; 
	GraphColoringInterface * g = new GraphColoringInterface();

	//Read a matrix from an input file and generate a corresponding graph.
	//The input format will be determined based on the file extension and a correct reading routine will be used to read the file.
	//Note: the input matrix MUST be SYMMETRIC in order for a graph to be generated correctly
	//		If you are new to COLPACK, pick either a .graph file (MeTiS format) or a symmetric .mtx (Matrix Market format)
	if ( g->ReadAdjacencyGraph(s_InputFile) == _FALSE) {
		cout<<"ReadAdjacencyGraph() Failed!!!"<<endl;
		return _FALSE;
	}
	cout<<"Done with ReadAdjacencyGraph()"<<endl;
	//Pause();

	//(Distance-2)Color the graph using "LARGEST_FIRST" Ordering. Other coloring and ordering can also be used.
	g->Coloring("DISTANCE_TWO", "LARGEST_FIRST");
	cout<<"Done with Coloring()"<<endl;
	//Pause();

	//Print coloring results
	g->PrintVertexColoringMetrics();
	cout<<"Done with PrintVertexColoringMetrics()"<<endl;
	delete g;
	//Pause();

	return _TRUE;
}
//*/