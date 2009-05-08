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

#ifndef BIPARTITEGRAPHPARTIALCOLORING_H
#define BIPARTITEGRAPHPARTIALCOLORING_H

namespace ColPack
{
	/** @ingroup group21
	 *  @brief class BipartiteGraphPartialColoring in @link group21@endlink.

	 To be completed.
	 */
	class BipartiteGraphPartialColoring : public BipartiteGraphPartialOrdering
	{
	public: //DOCUMENTED

		/// Based on m_s_VertexColoringVariant, either PrintRowPartialColors() or PrintColumnPartialColors() will be called.
		void PrintPartialColors();

		/// Based on m_s_VertexColoringVariant, either PrintRowPartialColoringMetrics() or PrintColumnPartialColoringMetrics() will be called.
		void PrintPartialColoringMetrics();

		/// Based on m_s_VertexColoringVariant, either PrintRowPartialColoringMetrics() or PrintColumnPartialColoringMetrics() will be called.
		int CheckPartialDistanceTwoColoring();

		/// Based on m_s_VertexColoringVariant, either GetLeftVertexColors() or GetRightVertexColors() will be called.
		void GetVertexPartialColors(vector<int> &output);

		/// Based on m_s_VertexColoringVariant, either GetLeftSeedMatrix() or GetRightSeedMatrix() will be called.
		double** GetSeedMatrix(int* i_SeedRowCount, int* i_SeedColumnCount);

		///Generate and return the Left Seed matrix 
		/**Precondition:
		- the Graph has been colored by PartialDistanceTwoRowColoring()

		Postcondition:
		- Size of the returned matrix is (*i_SeedRowCount) rows x (*i_SeedColumnCount) columns.
		(*i_SeedColumnCount) == num of rows of the original matrix == GetRowVertexCount()
		(*i_SeedRowCount) == num of colors used to color the left (row) vertices == GetVertexColorCount().
		*/
		double** GetLeftSeedMatrix(int* i_SeedRowCount, int* i_SeedColumnCount);

		///Return the Right Seed matrix  
		/** Precondition:
		- the Graph has been colored by PartialDistanceTwoColumnColoring()

		Postcondition:
		- Size of the returned matrix is (*i_SeedRowCount) rows x (*i_SeedColumnCount) columns.
		(*i_SeedRowCount) == num of columns of the original matrix == GetColumnVertexCount()
		(*i_SeedColumnCount) == num of colors used to color the right (column) vertices == GetVertexColorCount().
		*/
		double** GetRightSeedMatrix(int* i_SeedRowCount, int* i_SeedColumnCount);
		
	private:

		//Private Function 2401
		int CalculateVertexColorClasses();
	
		//Private Function 2402
		int CheckVertexColoring(string s_VertexColoringVariant);

		protected:

		int m_i_LeftVertexColorCount;
		int m_i_RightVertexColorCount;

		int m_i_VertexColorCount;
		
		int m_i_ViolationCount;

		int m_i_ColoringUnits;

		int m_i_LargestLeftVertexColorClass;
		int m_i_LargestRightVertexColorClass;

		int m_i_LargestLeftVertexColorClassSize;
		int m_i_LargestRightVertexColorClassSize;

		int m_i_SmallestLeftVertexColorClass;
		int m_i_SmallestRightVertexColorClass;

		int m_i_SmallestLeftVertexColorClassSize;
		int m_i_SmallestRightVertexColorClassSize;

		double m_d_AverageLeftVertexColorClassSize;
		double m_d_AverageRightVertexColorClassSize;

		double m_d_ColoringTime;
		double m_d_CheckingTime; 

		string m_s_VertexColoringVariant;

		vector<int> m_vi_LeftVertexColors;
		vector<int> m_vi_RightVertexColors;

		vector<int> m_vi_LeftVertexColorFrequency;
		vector<int> m_vi_RightVertexColorFrequency;

		public:

		//Public Constructor 2451
		BipartiteGraphPartialColoring();

		//Public Destructor 2452
		~BipartiteGraphPartialColoring();

		//Virtual Function 2453
		virtual void Clear();

		//Virtual Function 2454
		virtual void Reset();

		//Public Function 2455
		int PartialDistanceTwoRowColoring();
	
		//Public Function 2456
		int PartialDistanceTwoColumnColoring();

		//Public Function 2457
		int CheckPartialDistanceTwoRowColoring();

		//Public Function 2458
		int CheckPartialDistanceTwoColumnColoring();

		//Public Function 2459
		int GetLeftVertexColorCount();

		//Public Function 2460
		int GetRightVertexColorCount();

		//Public Function 2461
		int GetVertexColorCount();

		//Public Function 2462
		string GetVertexColoringVariant();

		//Public Function 2463
		void GetLeftVertexColors(vector<int> &output);

		//Public Function 2464
		void GetRightVertexColors(vector<int> &output);

		//Public Function 2465
		void PrintRowPartialColors();

		//Public Function 2466
		void PrintColumnPartialColors();

		//Public Function 2467
		void PrintRowPartialColoringMetrics();

		//Public Function 2468
		void PrintColumnPartialColoringMetrics();

		//Public Function 2469
		void PrintVertexPartialColorClasses();
	};
}
#endif

