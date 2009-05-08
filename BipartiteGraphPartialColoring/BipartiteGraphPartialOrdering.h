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

#ifndef BIPARTITEGRAPHPARTIALORDERING_H
#define BIPARTITEGRAPHPARTIALORDERING_H

namespace ColPack
{
	/** @ingroup group21
	 *  @brief class BipartiteGraphPartialOrdering in @link group21@endlink.

	 The BipartiteGraphPartialOrderingClass stores either the ordered row or column vertices as a
	 vector of vertex identifiers to be used by bipartite graph partial coloring methods.
	 */
	class BipartiteGraphPartialOrdering : public BipartiteGraphInputOutput
	{
	public:

		int OrderVertices(string s_OrderingVariant = "NATURAL", string s_ColoringVariant = "PARTIAL_DISTANCE_TWO_COLUMN_COLORING");

	private:

		//Private Function 2301
		int CheckVertexOrdering(string s_VertexOrderingVariant);

	protected:

		double m_d_OrderingTime;

		string m_s_VertexOrderingVariant;

		vector<int> m_vi_OrderedVertices;

	public:

		//Public Constructor 2351
		BipartiteGraphPartialOrdering();

		//Public Destructor 2352
		~BipartiteGraphPartialOrdering();

		//Virtual Function 2353
		virtual void Clear();

		//Virtual Function 2354
		virtual void Reset();

		//Public Function 2355
		int RowNaturalOrdering();

		//Public Function 2356
		int ColumnNaturalOrdering();

		//Public Function 2357
		int RowLargestFirstOrdering();

		//Public Function 2358
		int ColumnLargestFirstOrdering();

		//Public Function 2359
		int RowSmallestLastOrdering();

		//Public Function 2360
		int ColumnSmallestLastOrdering();

		//Public Function 2361
		int RowIncidenceDegreeOrdering();

		//Public Function 2362
		int ColumnIncidenceDegreeOrdering();

		//Public Function 2363
		string GetVertexOrderingVariant();

		//Public Function 2364
		void GetOrderedVertices(vector<int> &output);
	};
}
#endif
