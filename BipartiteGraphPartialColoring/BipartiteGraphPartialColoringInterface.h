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

#ifndef BIPARTITEGRAPHPARTIALCOLORINGINTERFACE_H
#define BIPARTITEGRAPHPARTIALCOLORINGINTERFACE_H

namespace ColPack 
{
	/** @ingroup group21
	 *  @brief class BipartiteGraphPartialColoringInterface in @link group21@endlink.

	 To be completed.
	 Note that for each object, only one type of Coloring (either Row or Column) should be used.
	 The reason is because both of RowColoring and ColumnColoring will update (share) the value of m_i_VertexColorCount.
	 If RowColoring is run and then ColumnColoring is run, both PrintColumnPartialColoringMetrics() and PrintRowPartialColoringMetrics()
	 will display the result of the later run (ColumnColoring) only.
	 */
	class BipartiteGraphPartialColoringInterface : public BipartiteGraphPartialColoring
	{
	public: //DOCUMENTED

		/// Read bipartite graph structure from input file and (Partial-Distance-Two) color the bipartite graph
		/** This function will:
		- 0. Create initial BipartiteGraphPartialColoringInterface object
		- 1. Create the bipartite graph based on the graph structure specified by the input file
		- 2. Order the vertices based on the requested Ordering variant (s_OrderingVariant)
		- 3. (Partial-Distance-Two) Color the graph based on the requested Coloring variant (s_ColoringVariant)
		- Ordering Time and Coloring Time will be recorded.

		About input parameters:
		- fileName: name of the input file. If the full path is not given, the file is assumed to be in the current directory
			- If s_InputFile == "WAIT" (default), only step 0 will be done. 

		- fileType can be either:
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

		- s_OrderingVariant can be either:
			- "WAIT" (default), only step 0-1 will be execute. No ordering (step 2) will be done. 
			- "NATURAL"
			- "LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		- s_ColoringVariant can be either:
			- "WAIT" (default), only step 0-2 will be execute. No coloring (step 3) will be done. 
			- "RIGHT_PARTIAL_DISTANCE_TWO"
			- "LEFT_PARTIAL_DISTANCE_TWO"
		//*/
		BipartiteGraphPartialColoringInterface(string s_InputFile = "WAIT", string s_fileFormat="AUTO_DETECTED", string s_OrderingVariant = "WAIT", string s_ColoringVariant = "WAIT");

		/// (Partial-Distance-Two) Color the bipartite graph based on the requested s_ColoringVariant and s_OrderingVariant 
		/**	This function will
		- 1. Order the vertices based on the requested Ordering variant (s_OrderingVariant)
		- 2. Bicolor the graph based on the requested Coloring variant (s_ColoringVariant)
		- Ordering Time and Coloring Time will be recorded.
			
		About input parameters:
		- s_ColoringVariant can be either
			- "RIGHT_PARTIAL_DISTANCE_TWO" (default)
			- "LEFT_PARTIAL_DISTANCE_TWO"

		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		Postcondition:
		- The Bipartite Graph is (Partial-Distance-Two) colored, i.e., either m_vi_LeftVertexColors or m_vi_RightVertexColors will be populated.
		*/
		int PartialDistanceTwoColoring(string s_ColoringVariant = "RIGHT_PARTIAL_DISTANCE_TWO", string s_OrderingVariant = "NATURAL");

		/// Read the sparsity pattern of Jacobian matrix and return the corresponding seed matrix
		/**	This function will
		- 1. Read the sparsity pattern of the given Jacobian matrix (uip2_JacobianSparsityPattern)
		- 2. Create the corresponding bipartite graph
		- 3. Color the graph by (Row or Column)-Partial-Distance-2-Coloring  with the specified ordering
		- 4. Create and return the seed matrix (*dp3_seed) from the coloring information
			
		About input parameters:
		- The Jacobian matrix (uip2_JacobianSparsityPattern) must be stored in compressed sparse rows format
		- s_ColoringVariant:
			- if == "RIGHT_PARTIAL_DISTANCE_TWO" (default), PartialDistanceTwoColumnColoring() will be used and the right seed matrix will be return
			- if == "LEFT_PARTIAL_DISTANCE_TWO", PartialDistanceTwoRowColoring() will be used and the left seed matrix will be return
		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		Postcondition:
		- *dp3_seed: the size will be [*ip1_SeedRowCount] [*ip1_SeedColumnCount]
			- if (s_ColoringVariant == "RIGHT_PARTIAL_DISTANCE_TWO"): [num of columns of the original matrix == i_ColumnCount] [ColorCount]
			- if (s_ColoringVariant == "LEFT_PARTIAL_DISTANCE_TWO"): [ColorCount] [num of rows of the original matrix == i_RowCount]
		*/
		void GenerateSeedJacobian(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount, double*** dp3_seed, int *ip1_SeedRowCount, int *ip1_SeedColumnCount, string s_ColoringVariant = "RIGHT_PARTIAL_DISTANCE_TWO", string s_OrderingVariant="NATURAL");
		
	private:

		Timer m_T_Timer;

		public:
		
		//Public Destructor 2602
		~BipartiteGraphPartialColoringInterface();
	
		//Public Function 2603
		void Clear();

		//Public Function 2604
		void Reset();

		//Public Function 2605
		int RowNaturalOrderingPartialDistanceTwoRowColoring();

		//Public Function 2606
		int RowLargestFirstOrderingPartialDistanceTwoRowColoring();

		//Public Function 2607
		int RowSmallestLastOrderingPartialDistanceTwoRowColoring();

		//Public Function 2608
		int RowIncidenceDegreeOrderingPartialDistanceTwoRowColoring();

		//Public Function 2609
		int ColumnNaturalOrderingPartialDistanceTwoColumnColoring();

		//Public Function 2610
		int ColumnLargestFirstOrderingPartialDistanceTwoColumnColoring();

		//Public Function 2611
		int ColumnSmallestLastOrderingPartialDistanceTwoColumnColoring();

		//Public Function 2612
		int ColumnIncidenceDegreeOrderingPartialDistanceTwoColumnColoring();

	};
}
#endif

