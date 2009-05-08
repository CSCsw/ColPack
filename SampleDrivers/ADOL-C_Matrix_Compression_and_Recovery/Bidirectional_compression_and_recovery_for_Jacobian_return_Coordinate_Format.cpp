// An example of Bidirectional compression and recovery for Jacobian using Star Bicoloring
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

// baseDir should point to the directory (folder) containing the input file
string baseDir="C:\\Duck\\research\\ColPack\\Graphs\\";

#include "extra.h" //This .h file contains functions that are used in the below examples:
					//ReadMM(), MatrixMultiplication...(), Times2Plus1point5(), displayMatrix() and displayCompressedRowMatrix()

int main()
{
	// s_InputFile = baseDir + <name of the input file>
	string s_InputFile; //path of the input file
	s_InputFile = baseDir + "column-compress.mtx"; 

	// Step 1: Determine sparsity structure of the Jacobian.
	// This step is done by an AD tool. For the purpose of illustration here, we read the structure from a file, 
	// and store the structure in a Compressed Row Format. 
	unsigned int *** uip3_SparsityPattern = new unsigned int **;	//uip3_ means triple pointers of type unsigned int
	double*** dp3_Value = new double**;	//dp3_ means double pointers of type double. Other prefixes follow the same notation
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

	//Step 2: Obtain the seed matrices via Star Bicoloring.
	double*** dp3_LeftSeed = new double**;
	int *ip1_LeftSeedRowCount = new int;
	int *ip1_LeftSeedColumnCount = new int;
	double*** dp3_RightSeed = new double**;
	int *ip1_RightSeedRowCount = new int;
	int *ip1_RightSeedColumnCount = new int;
	BipartiteGraphBicoloringInterface *g = new BipartiteGraphBicoloringInterface;

	//Step 2.1: Read the sparsity pattern of the given Jacobian matrix (compressed sparse rows format)
	//and create the corresponding bipartite graph
	g->BuildBPGraph_ADOLC(*uip3_SparsityPattern, rowCount, columnCount);

	//Step 2.2: Color the graph based on the specified ordering and (Star) Bicoloring
	g->Bicoloring("IMPLICIT_COVERING__STAR_BICOLORING", "SMALLEST_LAST");

	//Step 2.3 (Option 1): From the coloring information, create and return the Left and Right seed matrices
	*dp3_LeftSeed = g->GetLeftSeedMatrix(ip1_LeftSeedRowCount, ip1_LeftSeedColumnCount);
	*dp3_RightSeed = g->GetRightSeedMatrix(ip1_RightSeedRowCount, ip1_RightSeedColumnCount);
	/* Notes:
	Step 2.3 (Option 2): From the coloring information, you can also get the vector of colorIDs of left and right vertices
		vector<int> vi_LeftVertexColors;
		g->GetLeftVertexColors(vi_LeftVertexColors);
		vector<int> RightVertexColors;
		g->GetRightVertexColors_Transformed(RightVertexColors);

	In stead of doing step 2.1-2.3 (Option 1), you can just call the bellow function:
		g->GenerateSeedJacobian(*uip3_SparsityPattern, rowCount, columnCount, dp3_LeftSeed, ip1_LeftSeedRowCount, ip1_LeftSeedColumnCount, dp3_RightSeed, ip1_RightSeedRowCount, ip1_RightSeedColumnCount, "IMPLICIT_COVERING__STAR_BICOLORING", "SMALLEST_LAST"); //???
	*/
	cout<<"Finish GenerateSeed()"<<endl;

	//Display results of step 2
	printf(" dp3_LeftSeed %d x %d", *ip1_LeftSeedRowCount, *ip1_LeftSeedColumnCount);
	displayMatrix(*dp3_LeftSeed, *ip1_LeftSeedRowCount, *ip1_LeftSeedColumnCount);
	printf(" dp3_RightSeed %d x %d", *ip1_RightSeedRowCount, *ip1_RightSeedColumnCount);
	displayMatrix(*dp3_RightSeed, *ip1_RightSeedRowCount, *ip1_RightSeedColumnCount);
	Pause();

	// Step 3: Obtain the Jacobian-RightSeed and LeftSeed-Jacobian matrix products.
	// This step will also be done by an AD tool. For the purpose of illustration here: 
	// - The left seed matrix LS is multiplied with the orginial matrix V (for Values).
	//   The resulting matrix is stored in dp3_LeftCompressedMatrix.
	// - The orginial matrix V (for Values) is multiplied with the right seed matrix RS.
	//   The resulting matrix is stored in dp3_RightCompressedMatrix.
	double*** dp3_LeftCompressedMatrix = new double**;
	double*** dp3_RightCompressedMatrix = new double**;
	cout<<"Start MatrixMultiplication() for both direction (left and right)"<<endl;
	MatrixMultiplication_SxV(*uip3_SparsityPattern, *dp3_Value, rowCount, columnCount, *dp3_LeftSeed, *ip1_LeftSeedRowCount, dp3_LeftCompressedMatrix);
	MatrixMultiplication_VxS(*uip3_SparsityPattern, *dp3_Value, rowCount, columnCount, *dp3_RightSeed, *ip1_RightSeedColumnCount, dp3_RightCompressedMatrix);
	cout<<"Finish MatrixMultiplication()"<<endl;

	displayMatrix(*dp3_RightCompressedMatrix,rowCount,*ip1_RightSeedColumnCount);
	displayMatrix(*dp3_LeftCompressedMatrix,*ip1_LeftSeedRowCount, columnCount);
	Pause();

	//Step 4: Recover the numerical values of the original matrix from the compressed representation. 
	// The new values are store in "dp3_NewValue"
	unsigned int** ip2_RowIndex = new unsigned int*;
	unsigned int** ip2_ColumnIndex = new unsigned int*;
	double** dp2_JacobianValue = new double*;
	JacobianRecovery2D::DirectRecover_CoordinateFormat(g, *dp3_LeftCompressedMatrix, *dp3_RightCompressedMatrix, *uip3_SparsityPattern, ip2_RowIndex, ip2_ColumnIndex, dp2_JacobianValue);
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
