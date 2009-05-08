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

#ifndef GRAPHCORE_H
#define GRAPHCORE_H
namespace ColPack
{
	/** @ingroup group1
	 *  @brief class GraphCore in @link group1@endlink.

	 Base class for Graph. Define a Graph: vertices, edges and values (edge's weight - optional); and its statisitcs: max, min and average degree.
	 */
	class GraphCore
	{
	public: //DOCUMENTED

		///Print all the Distance-1 neighbors of VertexIndex, except the excludedVertex
		void PrintVertexD1Neighbor(int VertexIndex, int excludedVertex = -1);

		/// Print all the Distance-2 neighbors of VertexIndex
		void PrintVertexD2Neighbor(int VertexIndex);

		/// Check and see if VertexIndex1 and VertexIndex2 are Distance-2 neighbor
		/** Algorithm:
		- Get the set D1_of_VertexIndex1 of all the Distance-1 neighbors of VertexIndex1
		- Get the set D1_of_VertexIndex2 of all the Distance-1 neighbors of VertexIndex2
		- Intersect D1_of_VertexIndex1 and D1_of_VertexIndex2 to see which vertices VertexIndex1 and VertexIndex2 have in common. The result is stored in Intersect_set
		- If the size of Intersect_set > 0 => VertexIndex1 and VertexIndex2 are Distance-2 neighbor
		*/
		bool AreD2Neighbor(int VertexIndex1, int VertexIndex2);

	protected:

		int m_i_MaximumVertexDegree;
		int m_i_MinimumVertexDegree;

		double m_d_AverageVertexDegree;

		string m_s_InputFile;

		vector<int> m_vi_Vertices;

		vector<int> m_vi_Edges;

		vector<double> m_vd_Values; //!< Edge's weight

		map< int, map< int, int> > m_mimi2_VertexEdgeMap; //moved from int GraphColoring::AcyclicColoring()
		DisjointSets m_ds_DisjointSets; //moved from int GraphColoring::AcyclicColoring()
	public:
		
		//Virtual Destructor 1100
		virtual ~GraphCore() {}

		//Virtual Function 1101
		virtual void Initialize() = 0;
		
		//Virtual Function 1102
		virtual void Clear();
		
		//Public Function 1103
		int GetVertexCount();

		
		//Public Function 1104
		int GetEdgeCount();

		
		//Public Function 1105
		int GetMaximumVertexDegree();

		
		//Public Function 1106
		int GetMinimumVertexDegree();

		
		//Public Function 1107
		double GetAverageVertexDegree();

		
		//Public Function 1108
		string GetInputFile();
		
		//Public Function 1109
		void GetVertices(vector<int> &output);
		
		//Public Function 1110
		void GetEdges(vector<int> &output);
		
		//Public Function 1111
		void GetValues(vector<double> &output);

		void GetVertexEdgeMap(map< int, map< int, int> > &output);

		void GetDisjointSets(DisjointSets &output);
		
	};
}
#endif

