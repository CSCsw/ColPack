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

#ifndef BIPARTITEGRAPHBICOLORINGINTERFACE_H
#define BIPARTITEGRAPHBICOLORINGINTERFACE_H

namespace ColPack 
{
	/** @ingroup group22
	 *  @brief class BipartiteGraphBicoloringInterface in @link group22@endlink.

	To be completed.
	*/
	class BipartiteGraphBicoloringInterface : public BipartiteGraphBicoloring
	{
	public: //DOCUMENTED
		/// Read bipartite graph structure from input file and Bicolor the bipartite graph
		/** This function will:
		- 0. Create initial BipartiteGraphPartialColoringInterface object
		- 1. Create the bipartite graph based on the graph structure specified by the input file
		- 2. Order the vertices based on the requested Ordering variant (s_OrderingVariant)
		- 3. Bicolor the graph based on the requested Bicoloring variant (s_BicoloringVariant)
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
			- "DYNAMIC_LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		- s_BicoloringVariant can be either:
			- "WAIT" (default), only step 0-2 will be execute. No coloring (step 3) will be done. 
			- "IMPLICIT_COVERING__STAR_BICOLORING"
			- "EXPLICIT_COVERING__STAR_BICOLORING"
			- "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING"
			- "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"
		//*/
		BipartiteGraphBicoloringInterface(string s_InputFile = "WAIT", string s_fileFormat="AUTO_DETECTED", string s_OrderingVariant = "WAIT", string s_BicoloringVariant = "WAIT");
	
		/// Read the sparsity pattern of Jacobian matrix and return the corresponding Left and Right Seed matrices
		/**	This function will
		- 1. Read the sparsity pattern of the given Jacobian matrix (uip2_JacobianSparsityPattern)
		- 2. Create the corresponding bipartite graph
		- 3. Color the graph based on the specified ordering and (Star) Bicoloring 
		- 4. From the coloring information, create and return the Left (*dp3_LeftSeed[*ip1_RowColorCount][i_RowCount]) and Right (*dp3_RightSeed[i_ColumnCount][*ip1_ColumnColorCount]) seed matrices 
			
		About input parameters:
		- The Jacobian matrix (uip2_JacobianSparsityPattern) must be stored in compressed sparse rows format
		- s_BicoloringVariant can be either
			- "IMPLICIT_COVERING__STAR_BICOLORING" (default)
			- "EXPLICIT_COVERING__STAR_BICOLORING"
			- "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING"
			- "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"
		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "DYNAMIC_LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		Postcondition:
		- *dp3_LeftSeed: the size will be:
			- Row count (*ip1_LeftSeedRowCount): Row Color Count
			- Column count (*ip1_LeftSeedColumnCount): Jacobian's Row Count
		- *dp3_RightSeed: the size will be:
			- Row count (*ip1_RightSeedRowCount): Jacobian's Column Count
			- Column count (*ip1_RightSeedColumnCount): Column Color Count
		*/
		void GenerateSeedJacobian(unsigned int ** uip2_JacobianSparsityPattern, int i_RowCount, int i_ColumnCount, double*** dp3_LeftSeed, int *ip1_LeftSeedRowCount, int *ip1_LeftSeedColumnCount, double*** dp3_RightSeed, int *ip1_RightSeedRowCount, int *ip1_RightSeedColumnCount, string s_BicoloringVariant = "IMPLICIT_COVERING__STAR_BICOLORING", string s_OrderingVariant="NATURAL");


		/// Bicolor the bipartite graph based on the requested s_BicoloringVariant and s_OrderingVariant 
		/**	This function will
		- 1. Order the vertices based on the requested Ordering variant (s_OrderingVariant)
		- 2. Bicolor the graph based on the requested Bicoloring variant (s_BicoloringVariant)
		- Ordering Time and Coloring Time will be recorded.
			
		About input parameters:
		- s_BicoloringVariant can be either
			- "IMPLICIT_COVERING__STAR_BICOLORING" (default)
			- "EXPLICIT_COVERING__STAR_BICOLORING"
			- "EXPLICIT_COVERING__MODIFIED_STAR_BICOLORING"
			- "IMPLICIT_COVERING__GREEDY_STAR_BICOLORING"
			- "IMPLICIT_COVERING__ACYCLIC_BICOLORING"

		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "DYNAMIC_LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "INCIDENCE_DEGREE"

		Postcondition:
		- The Bipartite Graph is Bicolored, i.e., m_vi_LeftVertexColors and m_vi_RightVertexColors will be populated.
		*/
		int Bicoloring(string s_BicoloringVariant = "IMPLICIT_COVERING__STAR_BICOLORING", string s_OrderingVariant = "NATURAL");
		
	private:

		Timer m_T_Timer;

		
	public:
		//Public Destructor 3702
		~BipartiteGraphBicoloringInterface();
		
		//Virtual Function 3703
		virtual void Clear();
		
		//Virtual Function 3704
		virtual void Reset();

		//Public Function 3708
		int NaturalOrderingExplicitCoveringModifiedStarBicoloring();

		//Public Function 3709
		int LargestFirstOrderingExplicitCoveringModifiedStarBicoloring();

		//Public Function 3710
		int DynamicLargestFirstOrderingExplicitCoveringModifiedStarBicoloring();

		//Public Function 3711
		int SmallestLastOrderingExplicitCoveringModifiedStarBicoloring();

		//Public Function 3712
		int IncidenceDegreeOrderingExplicitCoveringModifiedStarBicoloring();

		//Public Function 3713
		int NaturalOrderingExplicitCoveringStarBicoloring();

		//Public Function 3714
		int LargestFirstOrderingExplicitCoveringStarBicoloring();

		//Public Function 3715
		int DynamicLargestFirstOrderingExplicitCoveringStarBicoloring();

		//Public Function 3716
		int SmallestLastOrderingExplicitCoveringStarBicoloring();

		//Public Function 3717
		int IncidenceDegreeOrderingExplicitCoveringStarBicoloring();

		//Public Function 3733
		int NaturalOrderingImplicitCoveringGreedyStarBicoloring();

		//Public Function 3734
		int LargestFirstOrderingImplicitCoveringGreedyStarBicoloring();

		//Public Function 3735
		int DynamicLargestFirstOrderingImplicitCoveringGreedyStarBicoloring();

		//Public Function 3736
		int SmallestLastOrderingImplicitCoveringGreedyStarBicoloring();

		//Public Function 3737
		int IncidenceDegreeOrderingImplicitCoveringGreedyStarBicoloring();

		//Public Function 3738
		int NaturalOrderingImplicitCoveringStarBicoloring();

		//Public Function 3739
		int LargestFirstOrderingImplicitCoveringStarBicoloring();

		//Public Function 3740
		int DynamicLargestFirstOrderingImplicitCoveringStarBicoloring();

		//Public Function 3741
		int SmallestLastOrderingImplicitCoveringStarBicoloring();

		//Public Function 3742
		int IncidenceDegreeOrderingImplicitCoveringStarBicoloring();

	};
}
#endif

