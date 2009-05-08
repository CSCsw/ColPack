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

#ifndef GRAPHCOLORING_H
#define GRAPHCOLORING_H

using namespace std;

namespace ColPack
{
	/** @ingroup group1
	 *  @brief class GraphColoring in @link group1@endlink.

	 Graph coloring is an assignment of consecutive integral numbers (each representing a color) to vertices,
	 edges or faces or a combination of two or more of these objects of a graph such that it satisfes one or more
	 constraints. The present version of ColPack provides methods for vertex coloring only. The minimum
	 number of vertex colors required to color a graph is known as the chromatic number of the graph. The
	 problem of finding the chromatic number for even a planar graph is NP-hard. ColPack features some of
	 the most efficient approximation algorithms available to date for some of the vertex coloring problems.
	 */
	class GraphColoring : public GraphOrdering
	{
	public: //DOCUMENTED

		///Return the Seed matrix  
		/** Precondition:
		- the Graph has been colored

		Postcondition:
		- Size of the returned matrix is (*ip1_SeedRowCount) rows x (*ip1_SeedColumnCount) columns.
		(*ip1_SeedRowCount) == num of columns of the original matrix == GetVertexCount()
		(*ip1_SeedColumnCount) == num of colors used to color vertices == GetVertexColorCount().
		*/
		double** GetSeedMatrix(int* ip1_SeedRowCount, int* ip1_SeedColumnCount);

		///Quick check to see if DistanceTwoColoring() ran correctly
		/** 
		Return value:
		- 1 when this function detects that DistanceTwoColoring() must have run INcorrectly.
		- 0 otherwise

		IMPORTANT: This is the quick check so if CheckQuickDistanceTwoColoring() return 1, 
		then DistanceTwoColoring() definitely ran INcorrectly.
		However, when CheckQuickDistanceTwoColoring() return 0, 
		it doesn't mean that DistanceTwoColoring() ran correctly (it may, it may not). 
		To be 100% sure, use CheckDistanceTwoColoring()
		
		Precondition: DistanceTwoColoring() has been run.

		Parameter: int Verbose
		- If Verbose == 0, this function only check and see if m_i_MaximumVertexDegree <= m_i_VertexColorCount + 1.
		- If Verbose == 1, this function will print out the vertex with m_i_MaximumVertexDegree where the error can be detected.
		- If Verbose == 2, this function will print out all the errors (violations) and then return.

		Algorithm:
		- See if m_i_MaximumVertexDegree <= STEP_UP(m_i_VertexColorCount).
		If DistanceTwoColoring() ran correctly, this should be the case
		- If m_i_MaximumVertexDegree > STEP_UP(m_i_VertexColorCount),
		DistanceTwoColoring() ran INcorrectly and this function will go ahead and
		find the 2 vertices within distance-2 have the same color
		*/
		int CheckQuickDistanceTwoColoring(int Verbose = 0);

		/// Check to see if DistanceTwoColoring() ran correctly
		/** 100% accurate but slow. For a quick check, use CheckQuickDistanceTwoColoring().

		Return value:
		- 1 when this function detects that DistanceTwoColoring() must have run INcorrectly.
		- 0 means DistanceTwoColoring() must have run correctly.
		
		Precondition: DistanceTwoColoring() has been run.

		Parameter: int Verbose
		- If Verbose == 0, this function will silently return after the first error is detected.
		- If Verbose == 1, this function will print out the error message and return after the first error is detected.
		- If Verbose == 2, this function will print out all the errors and then return.
		*/
		int CheckDistanceTwoColoring(int Verbose = 0);
	
	private:

		int m_i_ColoringUnits;

		//Private Function 1401
		int FindCycle(int, int, int, int, vector<int> &, vector<int> &, vector<int> &);

		//Private Function 1402
		int UpdateSet(int, int, int, map< int, map<int, int> > &, vector<int> &, vector<int> &, vector<int> &);

		//Private Function 1403
		int SearchDepthFirst(int, int, int, vector<int> &);

		//Private Function 1404
		int CheckVertexColoring(string s_GraphColoringVariant);

		//Private Function 1405
		int CalculateVertexColorClasses();

	protected:

		int m_i_VertexColorCount;

		int m_i_LargestColorClass;
		int m_i_SmallestColorClass;

		int m_i_LargestColorClassSize;
		int m_i_SmallestColorClassSize;

		double m_d_AverageColorClassSize;

		double m_d_ColoringTime;
		double m_d_CheckingTime;

		string m_s_VertexColoringVariant;

		vector<int> m_vi_VertexColors;

		vector<int> m_vi_VertexColorFrequency;

	public:
	
		//Public Constructor 1451
		GraphColoring();

		//Public Destructor 1452
		~GraphColoring();

		//Virtual Function 1453
		virtual void Clear();

		//Public Function 1454
		int DistanceOneColoring();

		//Public Function 1455
		int DistanceTwoColoring();

		//Public Function 1456
		int NaiveStarColoring();

		//Public Function 1457
		int RestrictedStarColoring();

		//Public Function 1458
		int StarColoring();

		//Public Function 1459
		int StarColoring(vector<int> &, vector<int> &, map< int, map<int, int> > &);

		//Public Function 1460
		int CheckStarColoring();

		//Public Function 1461
		/**
		Note: This function can not be used for recovery!
		*/
		int AcyclicColoring();

		//Public Function 1462
		/**
		Note: Originally created for Hessian Indirect Recovery
		*/
		int AcyclicColoring(vector<int> &, map< int, vector<int> > &);

		/**
		Note: Currently used for Hessian Indirect Recovery
		*/
		int AcyclicColoring_ForIndirectRecovery();

		//Public Function 1463
		int CheckAcyclicColoring();

		//Public Function 1464
		int TriangularColoring();

		//Public Function 1465
		int ModifiedTriangularColoring();

		//Public Function 1466
		int CheckTriangularColoring();

		//Public Function 1467
		string GetVertexColoringVariant();

		//Public Function 1468
		int GetVertexColorCount();

		//Public Function 1469
		void GetVertexColors(vector<int> &output);

		//Public Function 1470
		int GetHubCount();

		//Public Function 1471
		int GetSetCount();

		//Public Function 1472
		double GetVertexColoringTime();

		//Public Function 1473
		double GetVertexColoringCheckingTime();

		//Public Function 1474
		int PrintVertexColors();

		//Public Function 1475
		int FileVertexColors();

		//Public Function 1476
		int PrintVertexColoringMetrics();

		//Public Function 1477
		int FileVertexColoringMetrics();

		//Public Function 1478
		void PrintVertexColorClasses();
	};
}
#endif

