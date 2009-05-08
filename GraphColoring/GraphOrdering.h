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

#ifndef GRAPHORDERING_H
#define GRAPHORDERING_H

using namespace std;

namespace ColPack 
{
	/** @ingroup group1
	 *  @brief class GraphOrdering in @link group1@endlink.
	 
	 This class stores the ordered vertices as a vector of vertex identifiers to be used by coloring methods.
	 */
	class GraphOrdering : public GraphInputOutput
	{
	public:
		///Calculate and return the Maximum Back degree
		/** 
		Precondition: OrderVertices() has been called, i.e. m_vi_OrderedVertices has been populated
		Note: Back degree of a vertex is the degree of that vertex 
		in the subgraph consisting of vertices that had been ordered (i.e., the vertices that are ordered before the current vertex).
		Depend on the ordering style, each vertex in vector m_vi_OrderedVertices may have different Back degree.
		The Maximum Back degree of all vertices in the graph will be returned.
		This is the UPPER BOUND for the number of colors needed to D1-color the graph.
		//*/
		int GetMaxBackDegree();
		int GetMaxBackDegree2();

		int OrderVertices(string s_OrderingVariant);

	private:

		/// Get Back Degree of vertex m_vi_OrderedVertices[index]
		/**
		Precondition: OrderVertices() has been called, i.e. m_vi_OrderedVertices has been populated

		Note: This function is written quickly so it is not optimal
		//*/
		int GetBackDegree(int index);
		int GetBackDegree2(int index);

		//Private Function 1301
		int CheckVertexOrdering(string s_VertexOrderingVariant);

		int printVertexEdgeMap(vector< vector< pair< int, int> > > &vvpii_VertexEdgeMap);

	protected:

		double m_d_OrderingTime;

		string m_s_VertexOrderingVariant;

		vector<int> m_vi_OrderedVertices;

	public:

		//Public Constructor 1351
		GraphOrdering();

		//Public Destructor 1352
		~GraphOrdering();	

		//Virtual Function 1353
		virtual void Clear();

		//Public Function 1354
		int NaturalOrdering();

		//Public Function 1355
		int LargestFirstOrdering();

		//Public Function 1356
		int DynamicLargestFirstOrdering();

		//Public Function 1357
		int DistanceTwoLargestFirstOrdering();

		//Public Function 1358
		int SmallestLastOrdering();

		//Public Function 1359
		int DistanceTwoSmallestLastOrdering();

		//Public Function 1360
		int IncidenceDegreeOrdering();

		//Public Function 1361
		int DistanceTwoIncidenceDegreeOrdering();

		//Public Function 1362
		string GetVertexOrderingVariant();

		//Public Function 1363
		void GetOrderedVertices(vector<int> &output);

		//Public Function 1364
		double GetVertexOrderingTime();
	};
}
#endif


