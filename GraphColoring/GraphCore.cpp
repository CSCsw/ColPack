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

#include "ColPackHeaders.h"

using namespace std;

namespace ColPack
{
	//Virtual Function 1102
	void GraphCore::Clear() //!< Reinitialize all variables
	{
		m_i_MaximumVertexDegree = _UNKNOWN;
		m_i_MinimumVertexDegree = _UNKNOWN;
		
		m_d_AverageVertexDegree = _UNKNOWN;

		m_s_InputFile.clear();

		m_vi_Vertices.clear();
		m_vi_Edges.clear();

		m_vd_Values.clear();

		return;
	}
	
	//Public Function 1103
	int GraphCore::GetVertexCount() 
	{
		return(STEP_DOWN(m_vi_Vertices.size()));
	}

	
	//Public Function 1104
	int GraphCore::GetEdgeCount()
	{
		return(m_vi_Edges.size()/2);
	}

	
	//Public Function 1105
	int GraphCore::GetMaximumVertexDegree()
	{
		return(m_i_MaximumVertexDegree);
	}

	
	//Public Function 1106
	int GraphCore::GetMinimumVertexDegree()
	{
		return(m_i_MinimumVertexDegree);
	}

	
	//Public Function 1107
	double GraphCore::GetAverageVertexDegree()
	{
		return(m_d_AverageVertexDegree);
	}

	
	//Public Function 1108
	string GraphCore::GetInputFile()
	{
		return(m_s_InputFile);
	}
	
	//Public Function 1109
	void GraphCore::GetVertices(vector<int> &output)
	{
		output = (m_vi_Vertices);
	}

	
	//Public Function 1110
	void GraphCore::GetEdges(vector<int> &output)
	{
		output = (m_vi_Edges);
	}

	
	//Public Function 1111
	void GraphCore::GetValues(vector<double> &output)
	{
		output = (m_vd_Values);
	}

	void GraphCore::GetVertexEdgeMap(map< int, map< int, int> > &output)
	{
		output = (m_mimi2_VertexEdgeMap);
	}

	void GraphCore::GetDisjointSets(DisjointSets &output)
	{
		output = (m_ds_DisjointSets);
	}

	///Print all the Distance-1 neighbors of VertexIndex, except the excludedVertex
	void GraphCore::PrintVertexD1Neighbor(int VertexIndex, int excludedVertex) {
		if(VertexIndex > (int)m_vi_Vertices.size() - 2) {
			cout<<"Illegal request. VertexIndex > m_vi_Vertices.size() - 2"<<endl;
			return;
		}
		cout<<"Distance-1 neighbors of "<<VertexIndex<<" are: ";
		for(int i=m_vi_Vertices[VertexIndex]; i<m_vi_Vertices[STEP_UP(VertexIndex)]; i++) {
			if( excludedVertex == m_vi_Edges[i]) continue;
			cout<<m_vi_Edges[i]<<" ";
		}
		cout<<"( # of edges = "<<m_vi_Vertices[STEP_UP(VertexIndex)] - m_vi_Vertices[VertexIndex]<<")"<<endl;
	}

	/// Print all the Distance-2 neighbors of VertexIndex
	void GraphCore::PrintVertexD2Neighbor(int VertexIndex) {
		cout<<"--Distance-1 neighbors of "<<VertexIndex<<" are: --------------------------"<<endl;
		for(int i=m_vi_Vertices[VertexIndex]; i<m_vi_Vertices[STEP_UP(VertexIndex)]; i++) {
			PrintVertexD1Neighbor(m_vi_Edges[i], VertexIndex);
		}
		cout<<"----------------------------------------------------"<<endl;
	}

	/// Check and see if VertexIndex1 and VertexIndex2 are Distance-2 neighbor
	/** Algorithm:
	- Get the set D1_of_VertexIndex1 of all the Distance-1 neighbors of VertexIndex1
	- Get the set D1_of_VertexIndex2 of all the Distance-1 neighbors of VertexIndex2
	- Intersect D1_of_VertexIndex1 and D1_of_VertexIndex2 to see which vertices VertexIndex1 and VertexIndex2 have in common. The result is stored in Intersect_set
	- If the size of Intersect_set > 0 => VertexIndex1 and VertexIndex2 are Distance-2 neighbor
	*/
	bool GraphCore::AreD2Neighbor(int VertexIndex1, int VertexIndex2) {
		set<int> D1_of_VertexIndex1, D1_of_VertexIndex2;
		vector<int> Intersect_set;

		for(int i=m_vi_Vertices[VertexIndex1]; i<m_vi_Vertices[STEP_UP(VertexIndex1)]; i++)
			D1_of_VertexIndex1.insert(m_vi_Edges[i]);
		for(int i=m_vi_Vertices[VertexIndex2]; i<m_vi_Vertices[STEP_UP(VertexIndex2)]; i++)
			D1_of_VertexIndex2.insert(m_vi_Edges[i]);

		Intersect_set.resize(D1_of_VertexIndex1.size(),-1);
		set_intersection(D1_of_VertexIndex1.begin(), D1_of_VertexIndex1.end(),
						D1_of_VertexIndex2.begin(), D1_of_VertexIndex2.end(),
						Intersect_set.begin()	);
		int size = Intersect_set.size();
		while(Intersect_set[size-1] == -1 && size >= 1) size--;
		Intersect_set.resize(size,-1);


		if(size>0) {
			//Print
			printf("%d and %d connected through vertices: ", VertexIndex1, VertexIndex2);
			copy(Intersect_set.begin(), Intersect_set.end(), ostream_iterator<int>(cout, " "));
			cout << endl;
			return true;
		}
		return false;

		/*
		//Print
		printf("%d and %d connected through vertices: ", VertexIndex1, VertexIndex2);
		set_intersection(D1_of_VertexIndex1.begin(), D1_of_VertexIndex1.end(),
						D1_of_VertexIndex2.begin(), D1_of_VertexIndex2.end(),
						ostream_iterator<int>(cout, " ")	);    
		cout << endl;
		//*/
	}
	
}

