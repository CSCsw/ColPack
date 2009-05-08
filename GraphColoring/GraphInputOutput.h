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

#ifndef GRAPHINPUTOUTPUT_H
#define GRAPHINPUTOUTPUT_H

using namespace std;

namespace ColPack 
{
	/** @ingroup group1
	 *  @brief class GraphInputOutput in @link group1@endlink.

	 This class provides the input methods for reading in matrix or graph files in supported
	 formats for generating general graphs. Three input formats are supported by default - Matrix Market,
	 Harwell Boeing and MeTiS. None of the input methods judges file formats based on normal file extensions
	 (.mtx for Matrix Market, .rua for Harwell Boeing and .graph for MeTiS) and expects the right method
	 is used with the right input file format. Since this is the only place where a user input is required no
	 special error checking has been incorporated to avoid undetermined failures from wrong input file formats.
	 If non-zero matrix element values are read ColPack tries to preserve their original precisions.
	 */
	class GraphInputOutput : public GraphCore
	{
	public: //DOCUMENTED

		/// Read the matrix file and generate an adjacency graph
		/**	This function will
		- 1. Read the name of the matrix file and decide which matrix format the file used (based on the file extension). If the file name has no extension, the user will need to pass the 2nd parameter "fileType" explicitly to tell ColPack which matrix format is used 
		- 2. Call the corresponding reading routine to generate the graph
			
		About input parameters:
		- fileName: name of the input file. If the full path is not given, the file is assumed to be in the current directory
		- fileType can be either
			- "AUTO_DETECTED" (default) or "". ColPack will decide the format of the file based on the file extension:
				- ".mtx": symmetric MatrixMarket format
				- ".hb", or any combination of ".<r, c, p><s, u, h, x, r><a, e>": HarwellBoeing format
				- ".graph": MeTiS format
				- If the above extensions are not found, MatrixMarket format will be assumed.
			- "MM" for symmetric MatrixMarket format
			- "MM2" for general MatrixMarket format
			- "HB" for HarwellBoeing format
			- "MeTiS" for MeTiS format
		- Note: Notice that for any ".mtx" file, ReadMatrixMarketAdjacencyGraph() will be called by default, 
		assuming that this file has symmetric MatrixMarket format. If the file has general MatrixMarket format 
		but you know for sure that the matrix's structure is symmetric, you will need to explicitly pass "MM2" 
		as the second parameter.
		*/
		int ReadAdjacencyGraph(string s_InputFile, string s_fileFormat="AUTO_DETECTED");

		/// Read the sparsity pattern of Hessian matrix from ADOLC and build a corresponding graph
		/**
		Precondition:
		- The Hessian matrix must be stored in compressed sparse rows format

		Return value:
		- i_HighestDegree
		*/
		int BuildGraph_ADOLC(unsigned int ** uip2_HessianSparsityPattern, int i_RowCount);

		/// Read the MatrixMarket-format file and generate an adjacency graph
		/**
		Precondition:
		- s_InputFile should point to the MatrixMarket-format input file (file name usually ends with .mtx)
		- If (b_getStructureOnly == true) only the structure of the matrix is read.
		All the values for the non-zeros in the matrix will be ignored.

		ReadMatrixMarketAdjacencyGraph vs. ReadMatrixMarketAdjacencyGraph2:
		- The only different between these 2 rountines are the expected format for the input file.
		For example, for the following matrix:		<BR>
							<BR>
					1 0 1 0 <BR>
					0 1 0 0 <BR>
					1 0 1 0 <BR>
					0 0 0 1 <BR>

		- Input file for ReadMatrixMarketAdjacencyGraph() has a standard format 
		for "MatrixMarket matrix coordinate real symmetric," i.e. only the non-zeros 
		in the lower triangular are stored. The file should look like this:		<BR>
								<BR>
			1 1 <some value>	<BR>
			3 1 <some value>	<BR>
			2 2 <some value>	<BR>
			3 3 <some value>	<BR>
			4 4 <some value>	<BR>

		- Input file for ReadMatrixMarketAdjacencyGraph2() has a non-standard format 
		, i.e. all the non-zeros in the matrix are stored (kind of redundance).
		The file should look like this:		<BR>
								<BR>
			1 1 <some value>	<BR>
			3 1 <some value>	<BR>
			2 2 <some value>	<BR>
			1 3 <some value>	<BR>
			3 3 <some value>	<BR>
			4 4 <some value>	<BR>

		*/
		int ReadMatrixMarketAdjacencyGraph(string s_InputFile, bool b_getStructureOnly = false);

		/// Read the MatrixMarket-format file and generate an adjacency graph
		/**
		Precondition:
		- s_InputFile should point to the MatrixMarket-format input file (file name usually ends with .mtx)
		- If (b_getStructureOnly == true) only the structure of the matrix is read.
		All the values for the non-zeros in the matrix will be ignored.

		ReadMatrixMarketAdjacencyGraph vs. ReadMatrixMarketAdjacencyGraph2:
		- The only different between these 2 rountines are the expected format for the input file.
		For example, for the following matrix:		<BR>
							<BR>
					1 0 1 0 <BR>
					0 1 0 0 <BR>
					1 0 1 0 <BR>
					0 0 0 1 <BR>

		- Input file for ReadMatrixMarketAdjacencyGraph() has a standard format 
		for "MatrixMarket matrix coordinate real symmetric," i.e. the matrix is symmetric and only the non-zeros 
		in the lower triangular (including the diagonal) are stored. The file should look like this:		<BR>
								<BR>
			1 1 <some value>	<BR>
			3 1 <some value>	<BR>
			2 2 <some value>	<BR>
			3 3 <some value>	<BR>
			4 4 <some value>	<BR>

		- Input file for ReadMatrixMarketAdjacencyGraph2() has a non-standard format 
		, i.e. the matrix is assumed to be non-symmetric and all the non-zeros in the matrix are store
		(kind of redundance if the stored matrix is indeed a symmetric one).
		The file should look like this:		<BR>
								<BR>
			1 1 <some value>	<BR>
			3 1 <some value>	<BR>
			2 2 <some value>	<BR>
			1 3 <some value>	<BR>
			3 3 <some value>	<BR>
			4 4 <some value>	<BR>

		*/
		int ReadMatrixMarketAdjacencyGraph2(string s_InputFile, bool b_getStructureOnly = false);

		//Public Function 1257
		/// Read the MatrixMarket-format file and generate an adjacency graph (structure only)
		/** Equivalence to ReadMatrixMarketAdjacencyGraph(s_InputFile, true);
		*/
		int ReadMatrixMarketAdjacencyGraphStructure(string s_InputFile);

		/// Read the MatrixMarket-format file and generate an adjacency graph (structure only)
		/** Equivalence to ReadMatrixMarketAdjacencyGraph2(s_InputFile, true);
		*/
		int ReadMatrixMarketAdjacencyGraphStructure2(string s_InputFile);

	private:

		//Private Function 1201
		int ParseWidth(string FortranFormat);

		//Private Function 1202
		void CalculateVertexDegrees();

	public:

		//Public Constructor 1251
		GraphInputOutput();

		//Public Destructor 1252
		~GraphInputOutput();

		//Virtual Function 1253
		//??? What is the purpose of this function? TO BE IMPLEMENTED
		virtual void Initialize();

		//Virtual Function 1254
		virtual void Clear();

		//Public Function 1255
		string GetInputFile();

		/*
		//Public Function 1256
		int ReadMatrixMarketAdjacencyGraph(string s_InputFile);
		//*/

		//Public Function 1258
		int ReadHarwellBoeingAdjacencyGraph(string s_InputFile);

		//Public Function 1259
		int ReadMeTiSAdjacencyGraph(string s_InputFile);

		//Public Function 1260
		int PrintGraph();

		//Public Function 1261
		int PrintGraphStructure();

		//Public Function 1262
		int PrintMatrix();

		//Public Function 1263
		int PrintMatrix(vector<int> &, vector<int> &, vector<double> &);

		//Public Function 1264
		void PrintVertexDegrees();
	};
}
#endif

