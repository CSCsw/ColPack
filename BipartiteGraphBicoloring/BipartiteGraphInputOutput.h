/************************************************************************************
    Copyright (C) 2005-2008 Assefaw H. Gebremedhin, Arijit Tarafdar, Duc Nguyen,
    Alex Pothen

    This file is part of ColPack.

    ColPack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ColPack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with ColPack.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************************/

using namespace std;

#ifndef BIPARTITEGRAPHINPUTOUTPUT_H
#define BIPARTITEGRAPHINPUTOUTPUT_H

namespace ColPack
{
	/** @ingroup group2
	 *  @brief class BipartiteGraphInputOutput in @link group2@endlink.

	 BipartiteGraphInputOutput class provides the input methods for reading in matrix or graph files in
	 supported formats for generating bipartite graphs. Three input formats are supported by default - Matrix
	 Market, Harwell Boeing and MeTiS. This class is similar to the GraphInputOutput class discussed in Section
	 2.1 in functionalities with the difference that it stores bipartite graphs in CES scheme.
	 */
	class BipartiteGraphInputOutput : public BipartiteGraphCore
	{
	public: //DOCUMENTED

		/// Read the matrix file and generate a bipartite graph
		/**	This function will
		- 1. Read the name of the matrix file and decide which matrix format the file used (based on the file extension). If the file name has no extension, the user will need to pass the 2nd parameter "s_fileFormat" explicitly to tell ColPack which matrix format is used 
		- 2. Call the corresponding reading routine to generate the graph
			
		About input parameters:
		-  s_InputFile: name of the input file. If the full path is not given, the file is assumed to be in the current directory
		- s_fileFormat can be either
			- "AUTO_DETECTED" (default) or "". ColPack will decide the format of the file based on the file extension:
				- ".mtx": MatrixMarket format
				- ".hb", or any combination of ".<r, c, p><s, u, h, x, r><a, e>": HarwellBoeing format
				- ".graph": MeTiS format
				- ".gen": Generic Matrix format
				- ".gens": Generic Square Matrix format
				- If the above extensions are not found, MatrixMarket format will be assumed.
			- "MM" for MatrixMarket format
			- "HB" for HarwellBoeing format
			- "MeTiS" for MeTiS format
			- "GEN" for Generic Matrix format
			- "GENS" for Generic Square Matrix format
		*/
		int ReadBipartiteGraph(string  s_InputFile, string s_fileFormat="AUTO_DETECTED");

		/// Read the sparsity pattern of Jacobian matrix from ADOLC and build a corresponding bipartite graph.
		/**
		Equivalent to CompressedSparseRowFormat2BipartiteGraph
		*/
		int BuildBPGraph_ADOLC(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount);

		/// Read the sparsity pattern of Jacobian matrix and build a corresponding bipartite graph
		/**
		Precondition:
		- The Jacobian matrix must be stored in compressed sparse rows format

		Return value:
		- _TRUE upon successful
		*/
		int CompressedSparseRowFormat2BipartiteGraph(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount);

		/// Output the structure (sparsity pattern) of Jacobian matrix based on the internal bipartite graph
		/**
		Postcondition:
		- The Jacobian matrix is in compressed sparse rows format
		- (*uip3_JacobianSparsityPattern) size is (GetRowVertexCount()) rows x (GetColumnVertexCount()) columns

		Return value:
		- _TRUE upon successful
		*/
		int BipartiteGraph2CompressedSparseRowFormat(unsigned int *** uip3_JacobianSparsityPattern, unsigned int * uip1_RowCount, unsigned int * uip1_ColumnCount);

		//Public Function 2258;3258
		/// Reading routine for generic matrix.
		/** The format of the matrix is specified bellow (this file format .gen is NOT the same as the .gen2 files used by ReadGenericSquareMatrixBipartiteGraph() ):
		- A matrix is specified row by row, each row ending with and endofline.
		- In each row, a nonzero is indicated by 1 and a zero is indicated by 0.
		- The number of rows, columns or nonzeros is not given in the file, but the filename indicates the number of rows and columns. Format: <matrix name>-<row>by<column>.gen
		- There are empty spaces between consecutive matrix entries.
		
		Example:	testmatrix-5by5.gen	<BR>
										<BR>
					1 1 1 0 1 0 1 0 1 0	<BR>
					0 1 0 1 0 1 0 1 0 1	<BR>
					1 0 1 1 1 0 1 0 1 0	<BR>
					0 1 0 1 0 1 0 1 0 1	<BR>
					1 0 1 0 1 1 1 0 1 0	<BR>
					0 1 0 1 0 1 0 1 0 1	<BR>
					1 0 1 0 1 0 1 1 1 0	<BR>
					0 1 0 1 0 1 0 1 0 1	<BR>
					1 0 1 0 1 0 1 0 1 1	<BR>
					0 1 0 1 0 1 0 1 0 1	<BR>
		*/
		int ReadGenericMatrixBipartiteGraph(string s_InputFile);

		//Public Function 2259;3259
		/// Reading routine for generic square matrix.
		/** The format of the matrix is specified bellow (this file format .gen2 is NOT the same as the .gen files used by ReadGenericMatrixBipartiteGraph() ):
		- The number of rows, columns or nonzeros is not given in the file, but the filename indicates the number of rows and columns. Format: <matrix name>-<row>by<column>.gens
		- NOTE: The number of rows should be equal to the number of columns. If the 2 numbers are different, take row = column = min of given row and column
		- The file contains a series of 0s and 1s with no space in between (endline should be ignore).
		- A nonzero is indicated by 1 and a zero is indicated by 0..
		
		Example:	testmatrix-12by10.gens (because the min is 10 => real size: 10x10)<BR>
																		<BR>
					11101010100101010101101110101001010101011010111010	<BR>
					01010101011010101110010101010110101010110101010101	<BR>
		*/
		int ReadGenericSquareMatrixBipartiteGraph(string s_InputFile);


	private:

		//Private Function 2201;3201
		void CalculateVertexDegrees();

	public:

		//Public Constructor 2251;3251
		BipartiteGraphInputOutput();

		//Public Destructor 2252;3252
		~BipartiteGraphInputOutput();

		//Virtual Function 2253;3254
		//??? What is the purpose of this function? TO BE IMPLEMENTED
		virtual void Initialize();

		//Virtual Function 2254;3254
		virtual void Clear();

		//Public Function 2255;3255
		int ReadMatrixMarketBipartiteGraph(string s_InputFile);

		//Public Function 2256;3256
		int ReadMeTiSBipartiteGraph(string s_InputFile);

		//Public Function 2257;3257
		int ReadHarwellBoeingBipartiteGraph(string s_InputFile);

		//Public Function 2260;3260
		void PrintBipartiteGraph();

		//Public Function 2261;3261
		void PrintVertexDegrees();
	};
}
#endif
