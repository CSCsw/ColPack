// An example of Row compression and recovery for Jacobian
/* How to use this driver:
	To compile the code, replace the Main.cpp file in Main directory with this file
	and run "make" in ColPack installation directory

Return by recovery routine: three vectors in "Coordinate Format" (zero-based indexing)
http://www.intel.com/software/products/mkl/docs/webhelp/appendices/mkl_appA_SMSF.html#mkl_appA_SMSF_5
unsigned int** ip2_RowIndex
unsigned int** ip2_ColumnIndex
double** dp2_JacobianValue // corresponding non-zero values
//*/

#include "ColPackHeaders.h"

using namespace ColPack;
using namespace std;

#include "extra.h" //This .h file contains functions that are used in the below examples:
					//ReadMM(), MatrixMultiplication...(), Times2Plus1point5(), displayMatrix() and displayCompressedRowMatrix()

int main()
{
	// s_InputFile = baseDir + <name of the input file>
	string s_InputFile; //path of the input file
	s_InputFile = "C:\\Duck\\research\\ColPack\\Graphs\\mymatrix.mtx"; 

	// Step 1: Determine sparsity structure of the Jacobian.
	// This step is done by an AD tool. For the purpose of illustration here, we read the structure from a file, 
	// and store the structure in a Compressed Row Format. 
	unsigned int *** uip3_SparsityPattern = new unsigned int **;
	double*** dp3_Value = new double**;
	int rowCount, columnCount;
	ConvertGeneralMatrixMarketFormatToCompressedSparseRowFormat(s_InputFile, uip3_SparsityPattern, dp3_Value,rowCount, columnCount);

	cout<<"just for debugging purpose, display the 2 matrices: the matrix with SparsityPattern only and the matrix with Value"<<endl;
	cout<<fixed<<showpoint<<setprecision(2);
	cout<<"(*uip3_SparsityPattern)"<<endl;
	displayCompressedRowMatrix((*uip3_SparsityPattern),rowCount);
	cout<<"(*dp3_Value)"<<endl;
	displayCompressedRowMatrix((*dp3_Value),rowCount);
	cout<<"Finish ReadMatrixMarketAdjacencyGraph2CompressedSparseRowFormat()"<<endl;
	Pause();

	//Step 2: Obtain the seed matrix via coloring.
	double*** dp3_Seed = new double**;
	int *ip1_SeedRowCount = new int;
	int *ip1_SeedColumnCount = new int;
	int *ip1_ColorCount = new int; //The number of distinct colors used to color the graph
	BipartiteGraphPartialColoringInterface *g = new BipartiteGraphPartialColoringInterface;

	//Step 2.1: Read the sparsity pattern of the given Jacobian matrix (compressed sparse rows format)
	//and create the corresponding bipartite graph
	g->BuildBPGraph_ADOLC(*uip3_SparsityPattern, rowCount, columnCount);

	//Step 2.2: Do Partial-Distance-Two-Coloring the bipartite graph with the specified ordering
	g->PartialDistanceTwoColoring("LEFT_PARTIAL_DISTANCE_TWO", "SMALLEST_LAST");

	//Step 2.3 (Option 1): From the coloring information, create and return the seed matrix
	*dp3_Seed = g->GetSeedMatrix(ip1_SeedRowCount, ip1_SeedColumnCount);
	/* Notes:
	Step 2.3 (Option 2): From the coloring information, you can also get the vector of colorIDs of left or right vertices (depend on the s_ColoringVariant that you choose)
		vector<int> vi_VertexPartialColors;
		g->GetVertexPartialColors(vi_VertexPartialColors);

	In stead of doing step 2.1-2.3 (Option 1), you can just call the bellow function:
		g->GenerateSeedJacobian(*uip3_SparsityPattern, rowCount, columnCount, dp3_Seed, ip1_SeedRowCount, ip1_SeedColumnCount, "LEFT_PARTIAL_DISTANCE_TWO", "SMALLEST_LAST"); // compress rows. This function is inside BipartiteGraphPartialColoringInterface class
	*/
	cout<<"Finish GenerateSeed()"<<endl;
	*ip1_ColorCount = *ip1_SeedRowCount;

	//Display results of step 2
	printf(" dp3_Seed %d x %d", *ip1_SeedRowCount, *ip1_SeedColumnCount);
	displayMatrix(*dp3_Seed, *ip1_SeedRowCount, *ip1_SeedColumnCount);
	Pause();

	// Step 3: Obtain the seed-Jacobian matrix product.
	// This step will also be done by an AD tool. For the purpose of illustration here, the seed matrix S 
	// is multiplied with the orginial matrix V (for Values). The resulting matrix is stored in dp3_CompressedMatrix.
	double*** dp3_CompressedMatrix = new double**;
	cout<<"Start MatrixMultiplication()"<<endl;
	MatrixMultiplication_SxV(*uip3_SparsityPattern, *dp3_Value, rowCount, columnCount, *dp3_Seed, *ip1_ColorCount, dp3_CompressedMatrix);
	cout<<"Finish MatrixMultiplication()"<<endl;

	displayMatrix(*dp3_CompressedMatrix,*ip1_ColorCount,columnCount);
	Pause();

	//Step 4: Recover the numerical values of the original matrix from the compressed representation. 
	// The new values are store in "dp2_JacobianValue"
	unsigned int** ip2_RowIndex = new unsigned int*;
	unsigned int** ip2_ColumnIndex = new unsigned int*;
	double** dp2_JacobianValue = new double*;
	JacobianRecovery1D::RecoverForPD2RowWise_CoordinateFormat(g, *dp3_CompressedMatrix, *uip3_SparsityPattern, ip2_RowIndex, ip2_ColumnIndex, dp2_JacobianValue);
	cout<<"Finish Recover()"<<endl;

	cout<<endl<<"Display result, the structure and values should be similar to the original one"<<endl;
	cout<<"Display *ip2_RowIndex"<<endl;
	displayVector(*ip2_RowIndex,g->GetEdgeCount());
	cout<<"Display *ip2_ColumnIndex"<<endl;
	displayVector(*ip2_ColumnIndex, g->GetEdgeCount());
	cout<<"Display *dp2_JacobianValue"<<endl;
	displayVector(*dp2_JacobianValue, g->GetEdgeCount());
	Pause();
	return 0;
}
