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

#ifndef DISJOINTSETS_H
#define DISJOINTSETS_H

using namespace std;

namespace ColPack
{
	/** @ingroup group4
	 *  @brief class DisjointSets in @link group4@endlink.

	 The disjoint set class is used by ColPack to store and operate on disjoint sets of edges identified by
	 integer numbers. A disjoint set class can be instantiated by specifying the maximum number of such sets to
	 be stored. The elements in a set are stored as a tree and the identifier of the set is the identifier of the root.
	 The size of the tree is stored in the root and the parent of an element is stored in the element. The tree is
	 implemented simply as a vector of integers the indices being the identifiers of the elements.
	*/
	class DisjointSets
	{
	 private:
	   
		vector<int> p_vi_Nodes;
	   
	 public:
	    
		//Public Constructor 4251
		DisjointSets();

		//Public Constructor 4252
		DisjointSets(int);

		//Public Destructor 4253
		~DisjointSets();

		//Public Function 4254
		int SetSize(int);
	    
		//Public Function 4255
		int Count();
	    
		//Public Function 4256
		int Print();

		//Public Function 4257
		int Find(int);

		//Public Function 4258
		int FindAndCompress(int);

		//Public Function 4259
		int Union(int, int);

		//Public Function 4260
		int UnionByRank(int, int);

		//Public Function 4261
		int UnionBySize(int, int);
	    
	};
}
#endif
