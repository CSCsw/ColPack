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

#ifndef BIPARTITEGRAPHCORE_H
#define BIPARTITEGRAPHCORE_H

namespace ColPack
{
	/** @ingroup group2
	 *  @brief class BipartiteGraphCore in @link group2@endlink.

	 Base class for Bipartite Graph. Define a Bipartite Graph: left vertices, right vertices and edges; and its statisitcs: max, min and average degree.
	*/
	class BipartiteGraphCore
	{
	public: //DOCUMENTED
		//Public Function 2108:3108
		/// LeftVertexCount = RowVertexCount = m_vi_LeftVertices.size() -1
		int GetRowVertexCount();
		/// LeftVertexCount = RowVertexCount = m_vi_LeftVertices.size() -1
		int GetLeftVertexCount();

		
		//Public Function 2109:3109
		/// RightVertexCount = ColumnVertexCount = m_vi_RightVertices.size() -1
		int GetColumnVertexCount();
		/// RightVertexCount = ColumnVertexCount = m_vi_RightVertices.size() -1
		int GetRightVertexCount();

		

	protected:

		int m_i_MaximumLeftVertexDegree;
		int m_i_MaximumRightVertexDegree;
		int m_i_MaximumVertexDegree;

		int m_i_MinimumLeftVertexDegree;
		int m_i_MinimumRightVertexDegree;
		int m_i_MinimumVertexDegree;

		double m_d_AverageLeftVertexDegree;
		double m_d_AverageRightVertexDegree;
		double m_d_AverageVertexDegree;

		string m_s_InputFile;

		vector<int> m_vi_LeftVertices;
		vector<int> m_vi_RightVertices;

		vector<int> m_vi_Edges;

		map< int, map<int, int> > m_mimi2_VertexEdgeMap;
		

	public:
		
		//Virtual Function 2100:3100
		virtual ~BipartiteGraphCore(){}
		
		//Virtual Function 2101:3101
		virtual void Initialize() = 0;
	
		//Virtual Function 2102:3102
		virtual void Clear();
		
		//Public Function 2103:3103
		string GetInputFile();


		//Public Function 2104:3104
		void GetRowVertices(vector<int> &output);
		void GetLeftVertices(vector<int> &output);
		
		//Public Function 2105:3105
		void GetColumnVertices(vector<int> &output);
		void GetRightVertices(vector<int> &output);

		unsigned int GetRowVertices(unsigned int** ip2_RowVertex);
		unsigned int GetColumnIndices(unsigned int** ip2_ColumnIndex);
		
		//Public Function 2106:3106
		void GetEdges(vector<int> &output);
		
		//Public Function 2107:3107
		void GetVertexEdgeMap(map< int, map<int, int> > &output);

		
		//Public Function 2110:3110
		int GetEdgeCount();

		
		//Public Function 2111:3111
		int GetMaximumRowVertexDegree();

		
		//Public Function 2112:3112
		int GetMaximumColumnVertexDegree();

		
		//Public Function 2113:3113
		int GetMaximumVertexDegree();

		
		//Public Function 2114:3114
		int GetMinimumRowVertexDegree();

		
		//Public Function 2115:3115
		int GetMinimumColumnVertexDegree();

		
		//Public Function 2116:3116
		int GetMinimumVertexDegree();

		
		//Public Function 2117:3117
		double GetAverageRowVertexDegree();
		
		//Public Function 2118:3118
		double GetAverageColumnVertexDegree();
		
		//Public Function 2119:3119
		double GetAverageVertexDegree();
	};
}
#endif
