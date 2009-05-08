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

#ifndef GRAPHCOLORINGINTERFACE_H
#define GRAPHCOLORINGINTERFACE_H

using namespace std;

namespace ColPack 
{
	/** @ingroup group1
	 *  @brief class GraphColoringInterface in @link group1@endlink.
	 */
	class GraphColoringInterface : public GraphColoring
	{
		using GraphColoring::DistanceOneColoring;
		using GraphColoring::DistanceTwoColoring;
		using GraphColoring::NaiveStarColoring;
		using GraphColoring::RestrictedStarColoring;
		using GraphColoring::StarColoring;
		using GraphColoring::AcyclicColoring;
		using GraphColoring::ModifiedTriangularColoring;
		using GraphColoring::TriangularColoring;

	public: //DOCUMENTED

		//Public Constructor 1601
		/// Read graph structure from input file and color the adjacency graph
		/** This function will:
		- 0. Create initial GraphColoringInterface object
		- 1. Create the adjacency graph based on the graph structure specified by the input file
		- 2. Order the vertices based on the requested Ordering Method (s_OrderingVariant)
		- 3. Color the graph based on the requested Coloring Method (s_ColoringVariant)
		- Ordering Time and Coloring Time will be recorded.

		About input parameters:
		- fileName: name of the input file. If the full path is not given, the file is assumed to be in the current directory
			- If s_InputFile == "WAIT" (default), only step 0 will be done. 

		- fileType can be either:
			- "AUTO_DETECTED" (default) or "". ColPack will decide the format of the file based on the file extension:
				- ".mtx": symmetric MatrixMarket format
				- ".hb", or any combination of ".<r, c, p><s, u, h, x, r><a, e>": HarwellBoeing format
				- ".graph": MeTiS format
				- If the above extensions are not found, MatrixMarket format will be assumed.
				- Note: Notice that for any ".mtx" file, ReadMatrixMarketAdjacencyGraph() will be called by default, 
				assuming that this file has symmetric MatrixMarket format. If the file has general MatrixMarket format 
				but you know for sure that the matrix's structure is symmetric, you will need to explicitly pass "MM2" 
				as the second parameter.
			- "MM" for symmetric MatrixMarket format
			- "MM2" for general MatrixMarket format
			- "HB" for HarwellBoeing format
			- "MeTiS" for MeTiS format

		- s_OrderingVariant can be either:
			- "WAIT" (default), only step 0-1 will be execute. No ordering (step 2) will be done. 
			- "NATURAL"
			- "LARGEST_FIRST"
			- "DYNAMIC_LARGEST_FIRST"
			- "DISTANCE_TWO_LARGEST_FIRST" (used primarily for DistanceTwoColoring and various StarColoring)
			- "SMALLEST_LAST"
			- "DISTANCE_TWO_SMALLEST_LAST" (used primarily for DistanceTwoColoring and various StarColoring)
			- "INCIDENCE_DEGREE"
			- "DISTANCE_TWO_INCIDENCE_DEGREE" (used primarily for DistanceTwoColoring and various StarColoring)

		- s_ColoringVariant can be either:
			- "WAIT" (default), only step 0-2 will be execute. No coloring (step 3) will be done. 
			- "DISTANCE_ONE"
			- "ACYCLIC"
			- "STAR"
			- "RESTRICTED_STAR"
			- "DISTANCE_TWO"
		//*/
		GraphColoringInterface(string s_InputFile = "WAIT", string s_fileFormat="AUTO_DETECTED", string s_OrderingVariant = "WAIT", string s_ColoringVariant = "WAIT");

		/// Color the adjacency graph based on the requested s_ColoringVariant and s_OrderingVariant 
		/**	This function will
		- 1. Order the vertices based on the requested Ordering Method (s_OrderingVariant)
		- 2. Color the graph based on the requested Coloring Method (s_ColoringVariant)
		- Ordering Time and Coloring Time will be recorded.
			
		About input parameters:
		- s_ColoringVariant can be either
			- "DISTANCE_ONE" (default)
			- "ACYCLIC"
			- "ACYCLIC_FOR_INDIRECT_RECOVERY"
			- "STAR"
			- "RESTRICTED_STAR"
			- "DISTANCE_TWO"

		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "DYNAMIC_LARGEST_FIRST"
			- "DISTANCE_TWO_LARGEST_FIRST" (used primarily for DistanceTwoColoring and various StarColoring)
			- "SMALLEST_LAST"
			- "DISTANCE_TWO_SMALLEST_LAST" (used primarily for DistanceTwoColoring and various StarColoring)
			- "INCIDENCE_DEGREE"
			- "DISTANCE_TWO_INCIDENCE_DEGREE" (used primarily for DistanceTwoColoring and various StarColoring)

		Postcondition:
		- The Graph is colored, i.e., m_vi_VertexColors will be populated.
		*/
		int Coloring(string s_ColoringVariant = "DISTANCE_ONE", string s_OrderingVariant = "NATURAL");

		/// Read the sparsity pattern of Hessian matrix and return the corresponding seed matrix
		/**	This function will
		- 1. Read the sparsity pattern of the given Hessian matrix (uip2_HessianSparsityPattern)
		- 2. Create the corresponding graph
		- 3. Color the graph based on the specified ordering and coloring
		- 4. Create and return the seed matrix (*dp3_seed) from the coloring information
			
		About input parameters:
		- The Hessian matrix (uip2_HessianSparsityPattern) must be stored in compressed sparse rows format
		- s_ColoringVariant can be either
			- "STAR" (default)
			- "RESTRICTED_STAR"
			- "ACYCLIC_FOR_INDIRECT_RECOVERY"
		- s_OrderingVariant can be either
			- "NATURAL" (default)
			- "LARGEST_FIRST"
			- "DYNAMIC_LARGEST_FIRST"
			- "DISTANCE_TWO_LARGEST_FIRST"
			- "SMALLEST_LAST"
			- "DISTANCE_TWO_SMALLEST_LAST"
			- "INCIDENCE_DEGREE"
			- "DISTANCE_TWO_INCIDENCE_DEGREE"

		Postcondition:
		- *dp3_seed: [(*ip1_SeedRowCount) == num of cols of the original matrix == i_RowCount (because Hessian is a square matrix)] [(*ip1_SeedColumnCount) == ColorCount]
		*/
		void GenerateSeedHessian(unsigned int ** uip2_HessianSparsityPattern, int i_RowCount, double*** dp3_seed, int *ip1_SeedRowCount, int *ip1_SeedColumnCount, string s_ColoringVariant="STAR", string s_OrderingVariant="NATURAL");

	private:

		Timer m_T_Timer;

		//Private Function 1502
		int NaturalOrderingDistanceOneColoring();

		//Private Function 1503
		int NaturalOrderingDistanceTwoColoring();

		//Private Function 1504
		int NaturalOrderingNaiveStarColoring();

		//Private Function 1505
		int NaturalOrderingRestrictedStarColoring();

		//Private Function 1506
		int NaturalOrderingStarColoring();

		//Private Function 1507
		int NaturalOrderingAcyclicColoring();

		//Private Function 1508
		int NaturalOrderingTriangularColoring();

		//Private Function 1509
		int LargestFirstOrderingDistanceOneColoring();

		//Private Function 1510
		int LargestFirstOrderingDistanceTwoColoring();

		//Private Function 1511
		int LargestFirstOrderingNaiveStarColoring();

		//Private Function 1512
		int LargestFirstOrderingRestrictedStarColoring();

		//Private Function 1513
		int LargestFirstOrderingStarColoring();

		//Private Function 1514
		int LargestFirstOrderingAcyclicColoring();

		//Private Function 1515
		int LargestFirstOrderingTriangularColoring();

		//Private Function 1516
		int DynamicLargestFirstOrderingDistanceOneColoring();

		//Private Function 1517
		int DynamicLargestFirstOrderingDistanceTwoColoring();

		//Private Function 1517
		int DynamicLargestFirstOrderingNaiveStarColoring();

		//Private Function 1518
		int DynamicLargestFirstOrderingRestrictedStarColoring();

		//Private Function 1519
		int DynamicLargestFirstOrderingStarColoring();

		//Private Function 1520
		int DynamicLargestFirstOrderingAcyclicColoring();

		//Private Function 1521
		int DynamicLargestFirstOrderingTriangularColoring();

		//Private Function 1522
		int DistanceTwoLargestFirstOrderingDistanceOneColoring();

		//Private Function 1523
		int DistanceTwoLargestFirstOrderingDistanceTwoColoring();

		//Private Function 1524
		int DistanceTwoLargestFirstOrderingNaiveStarColoring();

		//Private Function 1525
		int DistanceTwoLargestFirstOrderingRestrictedStarColoring();

		//Private Function 1526
		int DistanceTwoLargestFirstOrderingStarColoring();

		//Private Function 1527
		int DistanceTwoLargestFirstOrderingAcyclicColoring();

		//Private Function 1528
		int DistanceTwoLargestFirstOrderingTriangularColoring();

		//Private Function 1529
		int SmallestLastOrderingDistanceOneColoring();

		//Private Function 1530
		int SmallestLastOrderingDistanceTwoColoring();

		//Private Function 1531
		int SmallestLastOrderingNaiveStarColoring();

		//Private Function 1532
		int SmallestLastOrderingRestrictedStarColoring();

		//Private Function 1533
		int SmallestLastOrderingStarColoring();

		//Private Function 1534
		int SmallestLastOrderingAcyclicColoring();

		//Private Function 1535
		int SmallestLastOrderingTriangularColoring();

		//Private Function 1536
		int DistanceTwoSmallestLastOrderingDistanceOneColoring();

		//Private Function 1537
		int DistanceTwoSmallestLastOrderingDistanceTwoColoring();

		//Private Function 1538
		int DistanceTwoSmallestLastOrderingNaiveStarColoring();

		//Private Function 1539
		int DistanceTwoSmallestLastOrderingRestrictedStarColoring();

		//Private Function 1540
		int DistanceTwoSmallestLastOrderingStarColoring();

		//Private Function 1541
		int DistanceTwoSmallestLastOrderingAcyclicColoring();

		//Private Function 1542
		int DistanceTwoSmallestLastOrderingTriangularColoring();

		//Private Function 1543
		int IncidenceDegreeOrderingDistanceOneColoring();

		//Private Function 1544
		int IncidenceDegreeOrderingDistanceTwoColoring();

		//Private Function 1545
		int IncidenceDegreeOrderingNaiveStarColoring();

		//Private Function 1546
		int IncidenceDegreeOrderingRestrictedStarColoring();

		//Private Function 1547
		int IncidenceDegreeOrderingStarColoring();

		//Private Function 1548
		int IncidenceDegreeOrderingAcyclicColoring();

		//Private Function 1549
		int IncidenceDegreeOrderingTriangularColoring();

		//Private Function 1550
		int DistanceTwoIncidenceDegreeOrderingDistanceOneColoring();

		//Private Function 1551
		int DistanceTwoIncidenceDegreeOrderingDistanceTwoColoring();

		//Private Function 1552
		int DistanceTwoIncidenceDegreeOrderingNaiveStarColoring();

		//Private Function 1553
		int DistanceTwoIncidenceDegreeOrderingRestrictedStarColoring();

		//Private Function 1554
		int DistanceTwoIncidenceDegreeOrderingStarColoring();

		//Private Function 1556
		int DistanceTwoIncidenceDegreeOrderingAcyclicColoring();

		//Private Function 1557
		int DistanceTwoIncidenceDegreeOrderingTriangularColoring();

		//Private Function 1558
		int CompareVertexColors();

	public:
		using GraphColoring::CheckStarColoring;

		//Public Destructor 1602
		~GraphColoringInterface();

		//Virtual Function 1603
		virtual void Clear();

		//Public Function 1604
		int DistanceOneColoring(string s_OrderingVariant);

		//Public Function 1605
		int DistanceTwoColoring(string s_OrderingVariant);

		//Public Function 1606
		int NaiveStarColoring(string s_OrderingVariant);

		//Public Function 1607
		int RestrictedStarColoring(string s_OrderingVariant);

		//Public Function 1608
		int StarColoring(string s_OrderingVariant);

		//Public Function 1609
		int AcyclicColoring(string s_OrderingVariant);

		int AcyclicColoring_ForIndirectRecovery(string s_OrderingVariant);

		//Public Function 1610
		int TriangularColoring(string s_OrderingVariant);

		//Public Function 1611
		int CompareVertexColorsWithNaturalOrdering();

		//Public Function 1612
		int CompareVertexColorsWithLargestFirstOrdering();

		//Public Function 1613
		int CompareVertexColorsWithDynamicLargestFirstOrdering();

		//Public Function 1614
		int CompareVertexColorsWithDistanceTwoLargestFirstOrdering();

		//Public Function 1615
		int CompareVertexColorsWithSmallestLastOrdering();

		//Public Function 1616
		int CompareVertexColorsWithDistanceTwoSmallestLastOrdering();

		//Public Function 1617
		int CompareVertexColorsWithIncidenceDegreeOrdering();

		//Public Function 1618
		int CompareVertexColorsWithDistanceTwoIncidenceDegreeOrdering();

		//Public Function 1619
		int CompareVertexOrdersWithDistanceOneColoring();

		//Public Function 1620
		int CompareVertexOrdersWithDistanceTwoColoring();

		//Public Function 1621
		int CompareVertexOrdersWithNaiveStarColoring();

		//Public Function 1622
		int CompareVertexOrdersWithRestrictedStarColoring();

		//Public Function 1623
		int CompareVertexOrdersWithStarColoring();

		//Public Function 1624
		int CompareVertexOrdersWithAcyclicColoring();

		//Public Function 1625
		int CompareVertexOrdersWithTriangularColoring();
		
		void PrintInducedVertexDegrees(int i, int i_HighestInducedVertexDegree, vector< list<int> > &vli_GroupedInducedVertexDegrees);
	};
}
#endif
